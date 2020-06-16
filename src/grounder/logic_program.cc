#include "logic_program.h"

#include <vector>

using namespace std;

void LogicProgram::insert_fact(Fact &f) {
    facts.push_back(f);
}

const vector<Fact> &LogicProgram::get_facts() const {
    return facts;
}

const vector<unique_ptr<RuleBase>> &LogicProgram::get_rules() const {
    return rules;
}

RuleBase &LogicProgram::get_rule_by_index(int index) {
    return *rules[index];
}

const Fact &LogicProgram::get_fact_by_index(int index) const {
    return facts[index];
}

size_t LogicProgram::get_number_of_facts() {
    return facts.size();
}

const std::string &LogicProgram::get_atom_by_index(int index) const {
    assert(map_index_to_atom.find(index) != map_index_to_atom.end());
    return map_index_to_atom.at(index);
}