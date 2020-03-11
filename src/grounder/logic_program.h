#ifndef GROUNDER__LOGIC_PROGRAM_H_
#define GROUNDER__LOGIC_PROGRAM_H_

#include "fact.h"
#include "object.h"
#include "rules.h"

#include <vector>
#include <unordered_map>

class LogicProgram {
 public:
  LogicProgram() = default;

  std::vector<Fact> facts;
  std::vector<Object> objects;
  std::vector<Rule> rules;

  // Maps index of an atom A to list of pairs (R, P) where R is a rule
  // with A in the body and P is the position of this atom in the condition
  std::unordered_map<int, std::vector<std::pair<int,int>>> rule_matcher;

  void set_facts(const std::vector<Fact> &f);
  void set_objects(const std::vector<Object> &o);
  void set_rules(const std::vector<Rule> &r);

  /*
   * Create data structure mapping each atom to the rules such that the atom
   * appears on the body (i.e., condition) of the rule.
   */
  void create_rule_matcher();


}; // class LogicProgram

#endif //GROUNDER__LOGIC_PROGRAM_H_
