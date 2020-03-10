#include "logic_program.h"

using namespace std;

void LogicProgram::set_facts(const std::vector<Fact> &f) {
  LogicProgram::facts = f;
}
void LogicProgram::set_objects(const std::vector<Object> &o) {
  LogicProgram::objects = o;
}
void LogicProgram::set_rules(const std::vector<Rule> &r) {
  LogicProgram::rules = r;
}

void LogicProgram::create_rule_matcher() {
  // Loop over rule conditions
  for (const auto& rule : LogicProgram::rules) {
    int cont = 0;
    for (const auto& condition : rule.conditions) {
      unordered_map<int, vector<pair<int,int>>>::const_iterator
          found = LogicProgram::rule_matcher.find(condition.predicate_index);
      if (found == LogicProgram::rule_matcher.end()) {
        // Not found
        LogicProgram::rule_matcher[condition.predicate_index] =
            vector<pair<int,int>>(1, pair<int,int>(rule.index, cont++));
      }
      else {
        LogicProgram::rule_matcher[condition.predicate_index].emplace_back(
            rule.index, cont++);
      }
    }
  }
}
