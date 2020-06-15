#include "logic_program.h"

#include <queue>
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

bool LogicProgram::is_new(Fact &new_fact,
                          unordered_set<Fact> &reached_facts) {
    auto insert_result = reached_facts.insert(new_fact);
    if (insert_result.second) {
        new_fact.set_fact_index();
        insert_fact(new_fact);
        return true;
    }
    return false;
}

bool LogicProgram::is_new(Fact &new_fact, std::map<int, FactBucket> &reached_facts) {
    int pred_idx = new_fact.get_predicate_index();
    auto insert_result = reached_facts[pred_idx].insert(new_fact.get_arguments());
    if (insert_result.second) {
        new_fact.set_fact_index();
        insert_fact(new_fact);
        return true;
    }
    return false;
}

const std::string &LogicProgram::get_atom_by_index(int index) const {
    assert(map_index_to_atom.find(index) != map_index_to_atom.end());
    return map_index_to_atom.at(index);
}