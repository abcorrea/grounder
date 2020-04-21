#ifndef GROUNDER__RULES_H_
#define GROUNDER__RULES_H_

#include "atom.h"
#include "fact.h"

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
    using key_t = std::vector<int>;
    using index_t = std::unordered_map<key_t,
                                       std::unordered_set<Fact>,
                                       boost::hash<key_t>>;

    Atom effect;
    std::vector<Atom> conditions;
    int index;
    int type;

    static int next_index;

    // Head has no free var (including nullary atom).
    // Set in 'set_map_head_vars_to_positions'
    bool ground_effect;

    std::vector<index_t> hash_table_indices;

    // Only need to keep track of this for product rules, the other ones are very
    // predictable and have a well-behaved structure
    std::vector<std::vector<Arguments>> reached_facts_per_condition;

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
        hash_table_indices(0),
        reached_facts_per_condition(0) {
        if (type==JOIN) {
            hash_table_indices.resize(2);
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

    void add_reached_fact_to_condition(const Arguments args, int position) {
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

    // Return all facts that match a given key for the atom in position i.
    // Only useful for join rules.
    const std::unordered_set<Fact> &get_facts_matching_key(const std::vector<int> &key,
                                                           int position);

    // Return atom in the ith position of the condition (body)
    const Atom &get_condition_by_position(int i);

    // Return the facts matching the SINGLE CONDITION IN THE iTH POSITION
    std::vector<Arguments> &get_reached_facts_of_condition(int i);

    // Return the facts matching EVERY CONDITION
    const std::vector<std::vector<Arguments>> &get_reached_facts_all_conditions() const;

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
