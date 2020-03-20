#ifndef GROUNDER__RULES_H_
#define GROUNDER__RULES_H_

#include "atom.h"
#include "fact.h"

#include <string>
#include <utility>
#include <unordered_set>

#include <boost/functional/hash.hpp>

/*
 * Rule: Class implementing the rules of the datalog program. Divided into
 * three distinct types:
 *
 * (0): Project rules: Unary rules where all variables in the head occur in
 * the body.
 *
 * (1): Join rules: Binary rules where all vars in the head occur in the body
 * and all variables in the body but not in the head occur in both atoms.
 *
 * (2): Product rules: Special rule for the rules which are not necessarily in
 * any format of the ones above. The goal rule always falls into this case.
 *
 */
#define PROJECT 0
#define JOIN 1
#define PRODUCT 2

class Rule {
  Atom effect;
  std::vector<Atom> conditions;
  int index;
  int type;

  // Head has no free var (including nullary atom).
  // Set in 'set_map_heard_vars_to_positions'
  bool ground_effect;

  using key_t = std::vector<int>;
  using index_t = std::unordered_map<key_t, std::unordered_set<Fact>, boost::hash<key_t>>;
  std::vector<index_t> hash_table_indices;

  // Only need to keep track of this for product rules, the other ones are very
  // predictable and have a well-behaved structure
  std::vector<std::vector<std::vector<int>>> reached_facts_per_condition;

  // A vector with two elements indicating the positions in which the variables
  // in the key occur in each respective rule of the body. If the first element
  // has X in it's 3rd position, then it means that the first rule of the body
  // has the third variable of the key in its Xth position.
  std::vector<std::vector<int>> position_of_matching_vars;
  std::vector<int> matches;

  // Map each free variable of the head to the position of the argument
  std::unordered_map<int, int> map_free_var_to_position;

  std::vector<int> computing_matching_variables();

 public:
    Rule(Atom effect, std::vector<Atom> c, int type) :
        effect(std::move(effect)),
        conditions(std::move(c)),
        index(next_index++),
        type(type),
        hash_table_indices(0),
        reached_facts_per_condition(0) {
      if (type == JOIN) {
        hash_table_indices.resize(2);
        matches = computing_matching_variables();

      } else if (type == PRODUCT) {
        reached_facts_per_condition.resize(conditions.size());
      }
    };

    /*
     * Creates a map between free variables and the position of arguments
     * and also sets the boolean variable checking if the rule is ground or
     * not.
     */
  void set_map_heard_vars_to_positions() {
    ground_effect = true;
    int position_counter = 0;
    for (const auto &eff : effect.get_arguments()) {
      if (eff < 0) {
        // Free variable
        ground_effect = false;
        map_free_var_to_position[eff] = position_counter;
      }
      ++position_counter;
    }
  }

  void add_reached_fact_to_condition(const std::vector<int> args, int position) {
    reached_facts_per_condition[position].push_back(args);
  }

  // Check if head has argument with variable index i
  bool head_has_variale(int i) const;

  bool head_is_ground() const;

  // Assume that "head_has_argument" returned true
  size_t get_head_position_of_arg(int arg) const;

  const Atom &get_effect() const;

  const std::vector<Atom> &get_conditions() const;

  int get_index() const;

  int get_type() const;

  const std::unordered_set<Fact> &get_hash_table_indices_by_index(const key_t& k, int i);

  // Insert a key with empty correspondence in the has.
  // Use this to guarantee that a key exists.
  // Only useful for join rules.
  void insert_key_in_hash(const std::vector<int> &key, int position);
  // Insert the fact to a given key of one of the hashes.
  // Run insert_key_in_hash first.
  // Only useful for join rules.
  void insert_fact_in_hash(const Fact& fact, const std::vector<int> &key, int position);

  // Return all facts that match a given key for the atom in position i.
  // Only useful for join rules.
  const std::unordered_set<Fact> &get_facts_matching_key(const std::vector<int> &key, int position);

  // Return atom in the ith position of the condition (body)
  const Atom &get_condition_by_position(int i);

  // Return the facts matching the SINGLE CONDITION IN THE iTH POSITION
  std::vector<std::vector<int>> &get_reached_facts_of_condition(int i);

  // Return the facts matching EVERY CONDITION
  const std::vector<std::vector<std::vector<int>>> &get_reached_facts_all_conditions() const;

  // Only useful for join rules.
  const std::vector<int> &get_position_of_matching_vars(int position) const;

  const std::vector<int> &get_matches() const;

  const std::unordered_map<int, int> &get_map_free_var_to_position() const;

  // Get arguments of the ith condition in the body
  const std::vector<int> &get_condition_arguments(int i) const {
    return conditions[i].get_arguments();
  }

  const std::vector<int> &get_effect_arguments() const {
    return effect.get_arguments();
  }


 protected:
  static int next_index;
};

#endif //GROUNDER__RULES_H_
