#ifndef GROUNDER_GROUNDERS_GROUNDER_FACTORY_H_
#define GROUNDER_GROUNDERS_GROUNDER_FACTORY_H_

#include "grounder.h"

#include "fast_downward_grounder.h"
#include "new_grounder.h"

#include <iostream>

#include <boost/algorithm/string.hpp>

class GrounderFactory {
public:
    static Grounder *new_grounder(const std::string &method) {
        std::cout << "Creating grounder..." << std::endl;
        if (boost::iequals(method, "fd")) {
            return new FastDownwardGrounder();
        }
        if (boost::iequals(method, "new")) {
            return new NewGrounder();
        }
        throw std::runtime_error(
            "Unexpected grounding method: \"" + method + "\"");
    }
};

#endif //GROUNDER_GROUNDERS_GROUNDER_FACTORY_H_
