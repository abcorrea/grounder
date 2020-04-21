#ifndef GROUNDER__RULES_H_
#define GROUNDER__RULES_H_

#include "atom.h"
#include "fact.h"
#include "join_hash_table.h"

#include <string>
#include <utility>
#include <unordered_set>

#include <boost/functional/hash.hpp>


class MapVariablePosition {
    // Class mapping free variables to positions of the head/effect
    std::unordered_map<int, int> mapping;

public:
    MapVariablePosition() = default;

    void create_map(const Atom &effect) {
        int position_counter = 0;
        for (const auto &eff : effect.get_arguments()) {
            if (eff < 0) {
                // Free variable
                mapping[eff] = position_counter;
            }
            ++position_counter;
        }
    }

    bool has_variable(int i) const {
        return (mapping.count(i) > 0);
    }

    size_t at(int i) const {
        return mapping.at(i);
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

    // Only need to keep track of this for product rules, the other ones are very
    // predictable and have a well-behaved structure
    std::vector<ReachedFacts> reached_facts_per_condition;

    // A vector with two elements indicating the positions in which the variables
    // in the key occur in each respective rule of the body. If the first element
    // has X in it's 3rd position, then it means that the first rule of the body
    // has the third variable of the key in its Xth position.
    std::vector<std::vector<int>> position_of_matching_vars;
    std::vector<int> matches;

    MapVariablePosition variable_position;

    std::vector<int> computing_matching_variables();


public:
    Rule(Atom eff, std::vector<Atom> c, int type) :
        effect(std::move(eff)),
        conditions(std::move(c)),
        index(next_index++),
        type(type),
        hash_table_indices(),
        reached_facts_per_condition(0) {
        if (type==JOIN) {
            matches = computing_matching_variables();

        } else if (type==PRODUCT) {
            reached_facts_per_condition.resize(conditions.size());
        }

        variable_position.create_map(effect);
        ground_effect = true;
        for (const auto &e : effect.get_arguments()) {
            if (e < 0) {
                ground_effect = false;
            }
        }

    };

    void add_reached_fact_to_condition(const Arguments& args, int position) {
        reached_facts_per_condition[position].push_back(args);
    }

    bool head_is_ground() const;

    int head_has_arg(int arg) const;

    const Atom &get_effect() const;

    const std::vector<Atom> &get_conditions() const;

    int get_index() const;

    int get_type() const;

    // Insert the fact to a given key of one of the hashes.
    // Only useful for join rules.
    void insert_fact_in_hash(const Fact &fact,
                             const std::vector<int> &key,
                             int position);

    const JoinHashEntry &get_facts_matching_key(const JoinHashKey &key,
                                                int position) {
        return hash_table_indices.get_entries(key, position);
    }

    ReachedFacts &get_reached_facts_of_condition(int i) {
        return reached_facts_per_condition[i];
    }

    // Return the facts matching EVERY CONDITION
    const std::vector<ReachedFacts> &get_reached_facts_all_conditions() const {
        return reached_facts_per_condition;
    }

    // Only useful for join rules.
    const std::vector<int> &get_position_of_matching_vars(int position) const;

    const std::vector<int> &get_matches() const;

    // Get arguments of the ith condition in the body
    const Arguments &get_condition_arguments(int i) const {
        return conditions[i].get_arguments();
    }

    const Arguments &get_effect_arguments() const {
        return effect.get_arguments();
    }
};

#endif //GROUNDER__RULES_H_
