#ifndef GROUNDER_GROUNDERS_NEW_GROUNDER_H_
#define GROUNDER_GROUNDERS_NEW_GROUNDER_H_

#include "fast_downward_grounder.h"

#include <iostream>

class NewGrounder : public FastDownwardGrounder  {
public:
    NewGrounder() {
        std::cout << "Using new grounding algorithm..." << std::endl;
    }

    int ground(LogicProgram &lp) override {
        // TODO Implement
        return FastDownwardGrounder::ground(lp);
    }

};

#endif //GROUNDER_GROUNDERS_NEW_GROUNDER_H_
