#ifndef GROUNDER_GROUNDERS_FAST_DOWNWARD_GROUNDER_H_
#define GROUNDER_GROUNDERS_FAST_DOWNWARD_GROUNDER_H_

#include <iostream>
#include <unordered_set>

#include "grounder.h"

class FastDownwardGrounder : public Grounder {
 public:
  FastDownwardGrounder() {
    std::cout << "Using Fast Downward grounding algorithm..." << std::endl;
  }

  int ground(LogicProgram &lp) override;

  static Fact project(const Rule &rule, const Fact &fact);
  static std::vector<Fact> join(Rule &rule, const Fact &fact, int position);
  static bool is_new(Fact &new_fact,
              std::unordered_set<Fact> &reached_facts,
              LogicProgram &lp);
};

#endif //GROUNDER_GROUNDERS_FAST_DOWNWARD_GROUNDER_H_
