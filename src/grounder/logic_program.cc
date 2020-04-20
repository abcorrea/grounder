#include "logic_program.h"

using namespace std;

void LogicProgram::set_facts(const vector<Fact> &f) {
    LogicProgram::facts = f;
}
void LogicProgram::set_objects(const vector<Object> &o) {
    LogicProgram::objects = o;
}
void LogicProgram::set_rules(const vector<Rule> &r) {
    LogicProgram::rules = r;
}

void LogicProgram::create_rule_matcher() {
    // Loop over rule conditions
    for (const auto &rule : LogicProgram::rules) {
        int cont = 0;
        for (const auto &condition : rule.get_conditions()) {
            rule_matcher.insert(condition.get_predicate_index(),
                                rule.get_index(),
                                cont++);
        }
    }
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

const vector<Rule> &LogicProgram::get_rules() const {
    return rules;
}

const unordered_map<int,
                    string> &LogicProgram::get_map_index_to_atom() const {
    return map_index_to_atom;
}

const RuleMatcher
&LogicProgram::get_rule_matcher() const {
    return rule_matcher;
}

const Matches
&LogicProgram::get_matched_rules(int index) const {
    assert(has_matched_rule(index));
    return rule_matcher.at(index);
}

Rule &LogicProgram::get_rule_by_index(int index) {
    return rules[index];
}

const Fact &LogicProgram::get_fact_by_index(int index) const {
    return facts[index];
}

bool LogicProgram::has_matched_rule(int index) const {
    return (rule_matcher.atom_has_matched_rules(index));
}

size_t LogicProgram::get_number_of_facts() {
    return facts.size();
}