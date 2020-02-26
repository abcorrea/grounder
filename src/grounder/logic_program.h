#ifndef GROUNDER__LOGIC_PROGRAM_H_
#define GROUNDER__LOGIC_PROGRAM_H_

#include <vector>
#include <unordered_map>

#include "fact.h"
#include "object.h"
#include "rules.h"

class LogicProgram {
 public:
  LogicProgram() = default;

  std::vector<Fact> facts;
  std::vector<Object> objects;
  std::vector<Rule> rules;

  // TODO set up maps between indices and objects/facts/rules

 private:
}; // class LogicProgram

#endif //GROUNDER__LOGIC_PROGRAM_H_
