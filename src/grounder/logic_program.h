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
    std::vector<RuleBase *> rules;
    std::unordered_map<int, std::string> map_index_to_atom;

public:
    LogicProgram() = default;

    void set_facts(const std::vector<Fact> &f);
    void set_objects(const std::vector<Object> &o);
    void set_rules(const std::vector<RuleBase *> &r);
    void set_map_index_to_atom(const std::unordered_map<int, std::string> &m);

    void insert_fact(Fact &f);

    const std::vector<Fact> &get_facts() const;
    const std::vector<Object> &get_objects() const;
    const std::vector<RuleBase *> &get_rules() const;

    const std::unordered_map<int, std::string> &get_map_index_to_atom() const;

    RuleBase &get_rule_by_index(int index);

    const Fact &get_fact_by_index(int index) const;

    size_t get_number_of_facts();

};

#endif
