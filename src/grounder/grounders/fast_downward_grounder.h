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

  static std::unordered_map<int, int>
   compute_mapping_free_vars(const Rule &rule, std::vector<int> &new_arguments);
  static Fact project(const Rule &rule, const Fact &fact);
  static std::vector<Fact> join(Rule &rule, const Fact &fact, int position);
  static bool is_new(Fact &new_fact,
                     std::unordered_set<Fact> &reached_facts,
                     LogicProgram &lp);
  static std::vector<Fact> product(Rule &rule,
                                   const Fact &fact,
                                   int position);
};

#endif //GROUNDER_GROUNDERS_FAST_DOWNWARD_GROUNDER_H_
