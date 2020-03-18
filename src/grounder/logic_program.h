#ifndef GROUNDER__LOGIC_PROGRAM_H_
#define GROUNDER__LOGIC_PROGRAM_H_

#include "fact.h"
#include "object.h"
#include "rules.h"

#include <vector>
#include <unordered_map>

class LogicProgram {
  std::vector<Fact> facts;
  std::vector<Object> objects;
  std::vector<Rule> rules;
  std::unordered_map<int, std::string> map_index_to_atom;

  // Maps index of an atom A to list of pairs (R, P) where R is a rule
  // with A in the body and P is the position of this atom in the condition
  std::unordered_map<int, std::vector<std::pair<int,int>>> rule_matcher;

 public:
  LogicProgram() = default;

  void set_facts(const std::vector<Fact> &f);

  void set_objects(const std::vector<Object> &o);

  void set_rules(const std::vector<Rule> &r);

  void set_map_index_to_atom(const std::unordered_map<int, std::string> &m);

  void insert_fact(Fact &f);

  const std::vector<Fact> &get_facts() const;

  const std::vector<Object> &get_objects() const;

  const std::vector<Rule> &get_rules() const;

  const std::unordered_map<int, std::string> &get_map_index_to_atom() const;

  const std::unordered_map<int, std::vector<std::pair<int, int>>>
  &get_rule_matcher() const;

  // Assume that there is at least one matched rule.
  // Call "has_matched_rule" first
  const std::vector<std::pair<int, int>> &get_matched_rules(int index) const;

  Rule &get_rule_by_index(int index);

  const Fact &get_fact_by_index(int index) const;

  bool has_matched_rule(int index) const;

  size_t get_number_of_facts();

  /*
   * Create data structure mapping each atom to the rules such that the atom
   * appears on the body (i.e., condition) of the rule.
   */
  void create_rule_matcher();


}; // class LogicProgram

#endif //GROUNDER__LOGIC_PROGRAM_H_
