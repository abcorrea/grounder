#ifndef GROUNDER_RULES_H
#define GROUNDER_RULES_H

#include "../atom.h"
#include "../fact.h"
#include "../join_structures.h"

#include <string>
#include <utility>
#include <unordered_set>


class MapVariablePosition {
    // Class mapping free variables to positions of the head/effect
    std::unordered_map<Term, int, boost::hash<Term>> mapping;

public:
    MapVariablePosition() = default;

    void create_map(const Atom &effect) {
        int position_counter = 0;
        for (const auto &eff : effect.get_arguments()) {
            if (!eff.is_object()) {
                // Free variable
                mapping[eff] = position_counter;
            }
            ++position_counter;
        }
    }

    bool has_variable(const Term &t) const {
        return (mapping.count(t) > 0);
    }

    size_t at(const Term &t) const {
        return mapping.at(t);
    }
};


class ReachedFacts {
    // We do not use a vector of Facts because the Fact class is more complex than
    // what we need here for this use case.
    std::vector<Arguments> facts;

public:
    ReachedFacts() = default;

    void push_back(const Arguments& args) {
        facts.push_back(args);
    }

    bool empty() const {
        return facts.empty();
    }

    std::vector<Arguments>::const_iterator begin() const {
        return facts.begin();
    }

    std::vector<Arguments>::const_iterator end() const {
        return facts.end();
    }

};


/*
 * Rule: Class implementing the rules of the datalog program. Divided into
 * three distinct types:
 *
 * Join rules: Binary rules where all vars in the head occur in the body
 * and all variables in the body but not in the head occur in both atoms.
 *
 * Product rules: Special rule for the rules which are not necessarily in
 * any format of the ones above. The goal rule always falls into this case.
 *
 * Project rules: Unary rules where all variables in the head occur in
 * the body.
 *
 */
enum RuleType {JOIN, PRODUCT, PROJECT};

class RuleBase {
protected:
    Atom effect;
    std::vector<Atom> conditions;
    int index;
    bool ground_effect;
    static int next_index;

    MapVariablePosition variable_position;


public:
    RuleBase(Atom eff, std::vector<Atom> c) :
        effect(std::move(eff)),
        conditions(std::move(c)),
        index(next_index++) {

        variable_position.create_map(effect);
        ground_effect = true;
        for (const auto &e : effect.get_arguments()) {
            if (!e.is_object()) {
                ground_effect = false;
            }
        }
    };

    virtual ~RuleBase() = default;

    bool head_is_ground() const {
        return ground_effect;
    }

    // TODO Introduce overload using only index and passing VARIABLE to has_variable
    int get_head_position_of_arg(const Term &arg) const {
        if (variable_position.has_variable(arg))
            return variable_position.at(arg);
        return -1;
    }

    const Atom &get_effect() const {
        return effect;
    }

    const std::vector<Atom> &get_conditions() const {
        return conditions;
    }

    int get_index() const {
        return index;
    }

    virtual int get_type() const = 0;

    const Arguments &get_condition_arguments(int i) const {
        return conditions[i].get_arguments();
    }

    const Arguments &get_effect_arguments() const {
        return effect.get_arguments();
    }
};


class JoinRule : public RuleBase {
    JoinHashTable hash_table_indices;
    JoiningVariables position_of_joining_vars;
public:
    JoinRule(Atom eff, std::vector<Atom> c)
        : RuleBase(std::move(eff), std::move(c)),
          position_of_joining_vars(conditions) {
    }

    virtual int get_type() const override {
        return JOIN;
    }

    void insert_fact_in_hash(const Fact &fact,
                                   const JoinHashKey &key,
                                   int position) {
        hash_table_indices.insert(fact, key, position);
    }

    const JoinHashEntry &get_facts_matching_key(const JoinHashKey &key,
                                                int position) {
        return hash_table_indices.get_entries(key, position);
    }

    const std::vector<int> &get_position_of_matching_vars(int condition) const {
        return position_of_joining_vars.get_joining_vars_of_condition(condition);
    }

    size_t get_number_joining_vars() const {
        return position_of_joining_vars.get_number_of_joining_vars();
    }

};


class ProductRule : public RuleBase {
    std::vector<ReachedFacts> reached_facts_per_condition;
public:
    ProductRule(Atom eff, std::vector<Atom> c)
        : RuleBase(std::move(eff), std::move(c)),
          reached_facts_per_condition(conditions.size()) {
    }

    virtual int get_type() const override {
        return PRODUCT;
    }

    void add_reached_fact_to_condition(const Arguments& args, int position) {
        reached_facts_per_condition[position].push_back(args);
    }

    ReachedFacts &get_reached_facts_of_condition(int i) {
        return reached_facts_per_condition[i];
    }

    const std::vector<ReachedFacts> &get_reached_facts_all_conditions() const {
        return reached_facts_per_condition;
    }
};


class ProjectRule : public RuleBase {
public:
    using RuleBase::RuleBase;

    virtual int get_type() const override {
        return PROJECT;
    }
};


#endif //GROUNDER_RULES_H
