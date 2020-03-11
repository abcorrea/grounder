#ifndef GROUNDER_GROUNDERS_GROUNDER_FACTORY_H_
#define GROUNDER_GROUNDERS_GROUNDER_FACTORY_H_

#include <iostream>
#include <boost/algorithm/string.hpp>


#include "grounder.h"
#include "fast_downward_grounder.h"

class GrounderFactory {
 public:
  static Grounder *new_grounder (const std::string &method) {
    std::cout << "Creating grounder..." << std::endl;
    if (boost::iequals(method, "fd")) {
            return new FastDownwardGrounder();
    }
    else {
      return nullptr;
    }
  }
};

#endif //GROUNDER_GROUNDERS_GROUNDER_FACTORY_H_
