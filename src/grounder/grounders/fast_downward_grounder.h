#ifndef GROUNDER_GROUNDERS_FAST_DOWNWARD_GROUNDER_H_
#define GROUNDER_GROUNDERS_FAST_DOWNWARD_GROUNDER_H_

#include <iostream>

#include "grounder.h"

class FastDownwardGrounder : public Grounder {
 public:
  FastDownwardGrounder() {
    std::cout << "Using Fast Downward grounding algorithm..." << std::endl;
  }

  int ground(LogicProgram &lp) override;

};

#endif //GROUNDER_GROUNDERS_FAST_DOWNWARD_GROUNDER_H_
