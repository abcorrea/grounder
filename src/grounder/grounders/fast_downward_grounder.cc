#include "fast_downward_grounder.h"

#include <vector>
#include <queue>
#include <unordered_set>

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
    Fact &current_fact = lp.facts[i];
    q.pop();
    for (const auto &m : lp.rule_matcher[current_fact.predicate_index]) {
      int rule_index = m.first;
      int position_in_the_body = m.second;
      // Projection rule - single condition in the body
      // TODO project and enqueue new fact

      // Join rule - two conditions in the body
      // TODO check for matching facts and enqueue them
      // TODO check if we can get rid of symmetry using position_in_the_body

      // Product rule - more than one condition without shared free vars
      // TODO check what python code does
    }
  }

  for (const Fact &f : lp.facts) {
    f.print_atom(lp.objects);
  }

  return 0;
}
