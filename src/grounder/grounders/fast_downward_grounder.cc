#include "fast_downward_grounder.h"

#include <vector>
#include <queue>

using namespace std;

int FastDownwardGrounder::ground(LogicProgram &lp) {

  unordered_set<Fact> reached_facts;
  queue<int> q;

  for (const Fact &f : lp.facts) {
    q.push(f.fact_index);
    reached_facts.insert(f);
  }

  while (!q.empty()) {
    int i = q.front();
    Fact current_fact = lp.facts[i];
    q.pop();
    for (const auto &m : lp.rule_matcher[current_fact.predicate_index]) {
      int rule_index = m.first;
      int position_in_the_body = m.second;
      if (lp.rules[rule_index].type == PROJECT) {
        // Projection rule - single condition in the body
        assert(position_in_the_body == 0);
        Fact new_fact = project(lp.rules[rule_index], current_fact);
        if (is_new(new_fact, reached_facts, lp)) {
          q.push(new_fact.fact_index);
        }
      } else if (lp.rules[rule_index].type == JOIN) {
        // Join rule - two conditions in the body
        assert(position_in_the_body <= 1);
        for (Fact new_fact : join(lp.rules[rule_index],
                                  current_fact,
                                  position_in_the_body))
          if (is_new(new_fact, reached_facts, lp)) {
            q.push(new_fact.fact_index);
          }
      } else if (lp.rules[rule_index].type == PRODUCT) {
        // Product rule - more than one condition without shared free vars
        // TODO check what python code does
        for (Fact new_fact : product(lp.rules[rule_index],
                                     current_fact,
                                     position_in_the_body))
          if (is_new(new_fact, reached_facts, lp)) {
            q.push(new_fact.fact_index);
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
  vector<int> new_arguments  = rule.effect.arguments;

  for (const auto &cond : rule.conditions) {
    int position_counter = 0;
    for (const auto &arg : cond.arguments) {
      if (rule.map_free_var_to_position.count(arg) > 0) {
        // Variable should NOT be projected away by this rule
        new_arguments[rule.map_free_var_to_position.at(arg)] =
            fact.arguments[position_counter];
      }
      ++position_counter;
    }
  }

  return Fact(move(new_arguments),
              rule.effect.predicate_index);
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
 * The function returns a list of acts.
 *
 */
vector<Fact> FastDownwardGrounder::join(Rule &rule,
                                        const Fact &fact,
                                        int position) {

  vector<Fact> facts;

  vector<int> key(rule.matches.size());
  for (int i : rule.position_of_matching_vars[position]) {
    key.push_back(fact.arguments[i]);
  }

  auto it = rule.hash_table_indices[position].emplace(key, unordered_set<Fact>());
  rule.hash_table_indices[position][key].insert(fact);

  // See comment in "project" about 'new_arguments' vector
  vector<int> new_arguments_persistent = rule.effect.arguments;

  int position_counter = 0;
  for (auto &arg : rule.conditions[position].arguments) {
    new_arguments_persistent[rule.map_free_var_to_position[arg]] =
        fact.arguments[position_counter++];
  }

  int inverse_position = (position + 1) % 2;
  for (const Fact &f : rule.hash_table_indices[inverse_position][key]) {
    vector<int> new_arguments = new_arguments_persistent;
    position_counter = 0;
    for (auto &arg : rule.conditions[inverse_position].arguments) {
      new_arguments[rule.map_free_var_to_position[arg]] =
          f.arguments[position_counter++];
    }
    facts.emplace_back(move(new_arguments),
                       rule.effect.predicate_index);
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

  // First: check that *all* other positions of the effect have at least one tuple
  rule.reached_facts_per_condition[position].push_back(fact.arguments);
  int c = 0;
  for (const auto &v : rule.reached_facts_per_condition) {
    if (v.empty() and c != position)
      return new_facts;
    c++;
  }

  // If there is one reachable ground atom for every condition and the head
  // is nullary, then simply trigger it.
  if (rule.effect.arguments.empty()) {
    new_facts.emplace_back(vector<int>(),
                           rule.effect.predicate_index);
    return new_facts;
  }

  // Second: start creating a base for the new effect atom based on the fact
  // that we are currently expanding

  // See comment in "project" about 'new_arguments' vector
  vector<int> new_arguments_persistent = rule.effect.arguments;

  int position_counter = 0;
  for (auto &arg : rule.conditions[position].arguments) {
    new_arguments_persistent[rule.map_free_var_to_position[arg]] =
        fact.arguments[position_counter++];
  }

  // Third: in this case, we just loop over the other conditions and its already
  // reached facts and instantiate all possibilities (i.e., cartesian product).
  // We do this using a queue
  queue<pair<vector<int>, int>> q;
  q.push(make_pair(new_arguments_persistent, 0));
  while (!q.empty()) {
    vector<int> current_args = q.front().first;
    int counter = q.front().second;
    q.pop();
    if (counter >= rule.conditions.size()) {
      new_facts.emplace_back(current_args,
                             rule.effect.predicate_index);
    } else if (counter == position) {
      // If it is the condition that we are currently reaching, we do not need
      // to consider the other tuples with this predicate
      q.push(make_pair(current_args, counter + 1));
    } else {
      for (const auto &assignment : rule.reached_facts_per_condition[counter]) {
        if (assignment.empty())
          continue;
        vector<int> new_arguments = current_args; // start as a copy
        int value_counter = 0;
        for (int arg : rule.conditions[counter].arguments) {
          assert (value_counter < assignment.size());
          new_arguments[rule.map_free_var_to_position[arg]] =
              assignment[value_counter];
          ++value_counter;
        }
        q.emplace(new_arguments, counter+1);
      }
    }
  }

  return new_facts;
}

bool FastDownwardGrounder::is_new(Fact &new_fact,
                                  unordered_set<Fact> &reached_facts,
                                  LogicProgram &lp) {
  if (reached_facts.count(new_fact) == 0) {
    new_fact.set_fact_index();
    lp.facts.push_back(new_fact);
    reached_facts.insert(new_fact);
    return true;
  }
  return false;
}

