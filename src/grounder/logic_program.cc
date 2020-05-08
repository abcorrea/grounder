#include "logic_program.h"

using namespace std;

void LogicProgram::set_facts(const vector<Fact> &f) {
    LogicProgram::facts = f;
}
void LogicProgram::set_objects(const vector<Object> &o) {
    LogicProgram::objects = o;
}
void LogicProgram::set_rules(vector<unique_ptr<RuleBase>> &&r) {
    LogicProgram::rules = std::move(r);
}

void LogicProgram::set_map_index_to_atom(const unordered_map<int, string> &m) {
    LogicProgram::map_index_to_atom = m;
}

void LogicProgram::insert_fact(Fact &f) {
    facts.push_back(f);
}

const vector<Fact> &LogicProgram::get_facts() const {
    return facts;
}

const vector<Object> &LogicProgram::get_objects() const {
    return objects;
}

const vector<unique_ptr<RuleBase>> &LogicProgram::get_rules() const {
    return rules;
}

const unordered_map<int,
                    string> &LogicProgram::get_map_index_to_atom() const {
    return map_index_to_atom;
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
