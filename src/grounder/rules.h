#ifndef GROUNDER_RULES_H
#define GROUNDER_RULES_H

#include "atom.h"
#include "fact.h"
#include "join_structures.h"

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

class Rule {

    Atom effect;
    std::vector<Atom> conditions;
    int index;
    int type;
    bool ground_effect;
    static int next_index;

    JoinHashTable hash_table_indices;
    JoiningVariables position_of_joining_vars;

    // Only need to keep track of this for product rules, the other ones are very
    // predictable and have a well-behaved structure
    std::vector<ReachedFacts> reached_facts_per_condition;

    MapVariablePosition variable_position;


public:
    Rule(Atom eff, std::vector<Atom> c, int type) :
        effect(std::move(eff)),
        conditions(std::move(c)),
        index(next_index++),
        type(type),
        hash_table_indices(),
        position_of_joining_vars(conditions),
        reached_facts_per_condition(0) {
        if (type==JOIN) {
            position_of_joining_vars = JoiningVariables(conditions);

        } else if (type==PRODUCT) {
            reached_facts_per_condition.resize(conditions.size());
        }

        variable_position.create_map(effect);
        ground_effect = true;
        for (const auto &e : effect.get_arguments()) {
            if (!e.is_object()) {
                ground_effect = false;
            }
        }
    };

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

    int get_type() const {
        return type;
    }

    const Arguments &get_condition_arguments(int i) const {
        return conditions[i].get_arguments();
    }

    const Arguments &get_effect_arguments() const {
        return effect.get_arguments();
    }

    void insert_fact_in_hash(const Fact &fact,
                                   const JoinHashKey &key,
                                   int position) {
        assert(type == JOIN);
        hash_table_indices.insert(fact, key, position);
    }

    const JoinHashEntry &get_facts_matching_key(const JoinHashKey &key,
                                                int position) {
        assert(type == JOIN);
        return hash_table_indices.get_entries(key, position);
    }

    const std::vector<int> &get_position_of_matching_vars(int condition) const {
        assert(type == JOIN);
        return position_of_joining_vars.get_joining_vars_of_condition(condition);
    }

    size_t get_number_joining_vars() const {
        assert(type == JOIN);
        return position_of_joining_vars.get_number_of_joining_vars();
    }

    void add_reached_fact_to_condition(const Arguments& args, int position) {
        assert(type == PRODUCT);
        reached_facts_per_condition[position].push_back(args);
    }

    ReachedFacts &get_reached_facts_of_condition(int i) {
        assert(type == PRODUCT);
        return reached_facts_per_condition[i];
    }

    const std::vector<ReachedFacts> &get_reached_facts_all_conditions() const {
        assert(type == PRODUCT);
        return reached_facts_per_condition;
    }
};

#endif //GROUNDER_RULES_H
