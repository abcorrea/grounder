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

int LogicProgram::get_index_of_atom(std::string &name) const {
    assert(map_atom_to_index.find(name) != map_atom_to_index.end());
    return map_atom_to_index.at(name);
}

bool LogicProgram::is_action_predicate(int i) {
    return action_indices.find(i) != action_indices.end();
}

int LogicProgram::get_index_of_object(std::string &name) const {
    assert(map_object_name_to_index.find(name) != map_object_name_to_index.end());
    return map_object_name_to_index.at(name);
}
