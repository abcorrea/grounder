#include "fast_downward_grounder.h"

#include <vector>
#include <queue>

using namespace std;

int FastDownwardGrounder::ground(LogicProgram &lp) {

    create_rule_matcher(lp);

    unordered_set<Fact> reached_facts;
    queue<int> q;

    for (const Fact &f : lp.get_facts()) {
        q.push(f.get_fact_index());
        reached_facts.insert(f);
    }

    while (!q.empty()) {
        int i = q.front();
        Fact current_fact = lp.get_fact_by_index(i);
        int predicate_index = current_fact.get_predicate_index();
        q.pop();
        if (rule_matcher.atom_has_matched_rules(predicate_index)) {
            for (const auto
                    &m : rule_matcher.get_matched_rules(predicate_index)) {
                int rule_index = m.get_rule();
                int position_in_the_body = m.get_position();
                Rule &rule = lp.get_rule_by_index(rule_index);
                if (rule.get_type()==PROJECT) {
                    // Projection rule - single condition in the body
                    assert(position_in_the_body==0);
                    Fact new_fact =
                        project(rule, current_fact);
                    if ((new_fact.get_predicate_index()!=-1) and
                        is_new(new_fact, reached_facts, lp)) {
                        q.push(new_fact.get_fact_index());
                    }
                } else if (rule.get_type()==JOIN) {
                    // Join rule - two conditions in the body
                    assert(position_in_the_body <= 1);
                    for (Fact new_fact : join(rule,
                                              current_fact,
                                              position_in_the_body))
                        if (is_new(new_fact, reached_facts, lp)) {
                            q.push(new_fact.get_fact_index());
                        }
                } else if (rule.get_type()==PRODUCT) {
                    // Product rule - more than one condition without shared free vars
                    for (Fact new_fact : product(rule,
                                                 current_fact,
                                                 position_in_the_body))
                        if (is_new(new_fact, reached_facts, lp)) {
                            q.push(new_fact.get_fact_index());
                        }
                }
            }
        }
    }

    return 0;
}

/*
 * Project a sequence of objects into another sequence. The head of the rule H
 * has free(H) <= free(B), where B is the rule body (condition).
 *
 * First, we map every (negative) index in the head to its given position.
 * Then, we loop over the single atom in the body and project the ones
 * that are in the head.  If there are constants in the head, we keep them
 * in the resulting fact when we create the mapping.
 *
 */
Fact FastDownwardGrounder::project(const Rule &rule, const Fact &fact) {


    // New arguments start as a copy of the head atom and we just replace the
    // free variables. Constants will remain intact.
    Arguments new_arguments = rule.get_effect_arguments();

    for (const auto &cond : rule.get_conditions()) {
        int position_counter = 0;
        for (const auto &arg : cond.get_arguments()) {
            int pos = rule.head_has_arg(arg);
            if (pos != -1) {
                // Variable should NOT be projected away by this rule
                new_arguments.set_value(pos,
                                        fact.argument(position_counter));
            } else if (arg >= 0) {
                // Constant instead of free var
                if (fact.argument(position_counter)!=arg) {
                    // constants do not match!
                    return Fact(Arguments(vector<int>(0)), -1);
                }
            }
            ++position_counter;
        }
    }

    return Fact(move(new_arguments),
                rule.get_effect().get_predicate_index());
}

/*
 * Compute the new facts produced by a join rule.
 *
 * The function starts by computing the fact restricted to the key elements
 * (i.e., elements that the free var matches with the other condition). Then,
 * it updates the hash tables.
 *
 * Next, it maps every free variable to its position in the head atom, similarly
 * as done in the projection, but without considering constants in the head
 * because these should not happen. (I guess.)
 *
 * Then, it computes the new ground head atom by performing first creating
 * the new atom with the values from the currently fact being expanded. Then,
 * it loops over all previously expanded facts matching the same key (the ones
 * in the hash-table) and completing the instantiation.
 *
 * The function returns a list of actions.
 *
 */
vector<Fact> FastDownwardGrounder::join(Rule &rule,
                                        const Fact &fact,
                                        int position) {

    vector<Fact> facts;

    vector<int> key;
    key.reserve(rule.get_matches().size());
    for (int i : rule.get_position_of_matching_vars(position)) {
        key.push_back(fact.argument(i));
    }

    // Insert the fact in the hash table of the key
    rule.insert_fact_in_hash(fact, key, position);

    // See comment in "project" about 'new_arguments' vector
    Arguments new_arguments_persistent = rule.get_effect_arguments();

    int position_counter = 0;
    for (auto &arg : rule.get_condition_arguments(position)) {
        int pos = rule.head_has_arg(arg);
        if (pos != -1) {
            new_arguments_persistent.set_value(pos,
                                               fact.argument(position_counter));
        }
        position_counter++;
    }

    const int inverse_position = (position + 1)%2;
    for (const Fact &f : rule.get_facts_matching_key(key, inverse_position)) {
        Arguments new_arguments = new_arguments_persistent;
        position_counter = 0;
        for (auto &arg : rule.get_condition_arguments(inverse_position)) {
            int pos = rule.head_has_arg(arg);
            if (pos != -1) {
                new_arguments.set_value(pos,
                                        f.argument(position_counter));
            }
            position_counter++;
        }
        facts.emplace_back(move(new_arguments),
                           rule.get_effect().get_predicate_index());
    }
    return facts;
}

/*
 * In product rules, none of the free variables join and there might be
 * several atoms in the body.
 *
 * In practice, this means two cenarios:
 *
 * (1) the head is empty;
 * (2) every free variable in the body is also in the head
 *
 */
vector<Fact> FastDownwardGrounder::product(Rule &rule,
                                           const Fact &fact,
                                           int position) {

    vector<Fact> new_facts;

    // Verify that if there is a ground object in the condition of this atom,
    // then it matches the fact being expanded
    int c = 0;
    for (const auto &arg : rule.get_condition_arguments(position)) {
        if (arg >= 0 and arg!=fact.argument(c)) {
            return new_facts;
        }
        ++c;
    }

    // Check that *all* other positions of the effect have at least one tuple
    rule.add_reached_fact_to_condition(fact.get_arguments(), position);
    c = 0;
    for (const auto &v : rule.get_reached_facts_all_conditions()) {
        if (v.empty() and c!=position)
            return new_facts;
        c++;
    }

    // If there is one reachable ground atom for every condition and the head
    // is nullary or has no free variable, then simply trigger it.
    //if (rule.get_effect_arguments().empty()) {
    if (rule.head_is_ground()) {
        new_facts.emplace_back(rule.get_effect_arguments(),
                               rule.get_effect().get_predicate_index());
        return new_facts;
    }

    // Second: start creating a base for the new effect atom based on the fact
    // that we are currently expanding

    // See comment in "project" about 'new_arguments' vector
    Arguments new_arguments_persistent = rule.get_effect_arguments();

    int position_counter = 0;
    for (auto &arg : rule.get_condition_arguments(position)) {
        int pos = rule.head_has_arg(arg);
        if (pos != -1) {
            new_arguments_persistent.set_value(pos,
                                               fact.argument(position_counter));
        }
        position_counter++;
    }

    // Third: in this case, we just loop over the other conditions and its already
    // reached facts and instantiate all possibilities (i.e., cartesian product).
    // We do this using a queue
    queue<pair<Arguments, int>> q;
    q.push(make_pair(new_arguments_persistent, 0));
    while (!q.empty()) {
        Arguments current_args = q.front().first;
        int counter = q.front().second;
        q.pop();
        if (counter >= int(rule.get_conditions().size())) {
            new_facts.emplace_back(current_args,
                                   rule.get_effect().get_predicate_index());
        } else if (counter==position) {
            // If it is the condition that we are currently reaching, we do not need
            // to consider the other tuples with this predicate
            q.push(make_pair(current_args, counter + 1));
        } else {
            for (const auto &assignment : rule.get_reached_facts_of_condition(
                counter)) {
                Arguments new_arguments = current_args; // start as a copy
                size_t value_counter = 0;
                for (int arg : rule.get_condition_arguments(counter)) {
                    assert (value_counter < assignment.size());
                    int pos = rule.head_has_arg(arg);
                    if (pos != -1) {
                        new_arguments.set_value(pos,
                                                assignment[value_counter]);
                    }
                    ++value_counter;
                }
                q.emplace(new_arguments, counter + 1);
            }
        }
    }

    return new_facts;
}

bool FastDownwardGrounder::is_new(Fact &new_fact,
                                  unordered_set<Fact> &reached_facts,
                                  LogicProgram &lp) {
    if (reached_facts.count(new_fact)==0) {
        new_fact.set_fact_index();
        reached_facts.insert(new_fact);
        lp.insert_fact(new_fact);
        return true;
    }
    return false;
}
