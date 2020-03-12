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
    current_fact.print_atom(lp.objects);
    for (const auto &m : lp.rule_matcher[current_fact.predicate_index]) {
      int rule_index = m.first;
      int position_in_the_body = m.second;
      if (lp.rules[rule_index].type == PROJECT) {
        // Projection rule - single condition in the body
        assert(position_in_the_body == 0);
        Fact new_fact  = project(lp.rules[rule_index], current_fact);
        if (is_new(new_fact, reached_facts, lp)) {
          q.push(new_fact.fact_index);
        }
      } else if (lp.rules[rule_index].type == JOIN) {
        // Join rule - two conditions in the body
        assert(position_in_the_body <= 1);
        // TODO check for matching facts and enqueue them
        // TODO check if we can get rid of symmetry using position_in_the_body
      } else if (lp.rules[rule_index].type == PRODUCT) {
        // Product rule - more than one condition without shared free vars
        // TODO check what python code does
      }
    }
  }

  return 0;
}

Fact FastDownwardGrounder::project(const Rule &rule, const Fact &fact) {

  vector<int> new_arguments(rule.effect.arguments.size());

  unordered_map<int, int> map_free_var_to_position;
  int position_counter = 0;
  for (const auto &eff : rule.effect.arguments) {
    if (eff < 0) {
      // Free variable
      map_free_var_to_position[eff] = position_counter;
    } else {
      // Constant -> keep it in the new fact
      new_arguments[position_counter] = eff;
    }
    ++position_counter;
  }

  for (const auto &cond : rule.conditions) {
    position_counter = 0;
    for (const auto &arg : cond.arguments) {
      if (map_free_var_to_position.count(arg) > 0) {
        // Variable should NOT be projected away by this rule
        new_arguments[map_free_var_to_position[arg]] = fact.arguments[position_counter];
      }
      ++position_counter;
    }
  }

  return Fact(move(new_arguments), rule.effect.predicate, rule.effect.predicate_index);
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
