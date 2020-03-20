#include "rules.h"

#include <algorithm>

using namespace std;

int Rule::next_index = 0;

vector<int> Rule::computing_matching_variables() {
  // Join rule should have only have two atoms in the conditions
  assert(conditions.size() == 2);

  vector<int> new_key;
  vector<vector<int>> positions(2);

  int pos1 = 0;
  for (int c : conditions[0].get_arguments()) {
    auto it2 = find(conditions[1].get_arguments().begin(), conditions[1].get_arguments().end(), c);
    if (it2 != conditions[1].get_arguments().end()) {
      // Free variables match in both atoms
      int pos2 = distance(conditions[1].get_arguments().begin(), it2);
      new_key.push_back(c);
      positions[0].push_back(pos1);
      positions[1].push_back(pos2);
    }
    ++pos1;
  }
  position_of_matching_vars = positions;
  return new_key;
}

bool Rule::head_has_variale(int i) const {
  return (map_free_var_to_position.count(i) > 0);
}

// Assume that "head_has_argument" returned true
size_t Rule::get_head_position_of_arg(int arg) const {
  return map_free_var_to_position.at(arg);
}

const Atom &Rule::get_effect() const {
  return effect;
}

const vector<Atom> &Rule::get_conditions() const {
  return conditions;
}

int Rule::get_index() const {
  return index;
}

int Rule::get_type() const {
  return type;
}

const std::unordered_set<Fact> &Rule::get_hash_table_indices_by_index(const key_t& k, int i)  {
  return hash_table_indices[i].at(k);
}

std::vector<std::vector<int>> &Rule::get_reached_facts_of_condition(int i) {
  return reached_facts_per_condition[i];
}

void Rule::insert_key_in_hash(const vector<int> &key, int position) {
  hash_table_indices[position].emplace(key, unordered_set<Fact>());
}

void Rule::insert_fact_in_hash(const Fact& fact, const vector<int> &key, int position) {
  hash_table_indices[position][key].insert(fact);
}

const vector<int> &Rule::get_position_of_matching_vars(int position) const {
  return position_of_matching_vars[position];
}
const vector<int> &Rule::get_matches() const {
  return matches;
}
const unordered_map<int, int> &Rule::get_map_free_var_to_position() const {
  return map_free_var_to_position;
}

const std::unordered_set<Fact> &Rule::get_facts_matching_key(const vector<int> &key,
                                                      int position) {
  return hash_table_indices[position][key];
}

const Atom &Rule::get_condition_by_position(int i) {
  return conditions[i];
}

const vector<std::vector<std::vector<int>>> &Rule::get_reached_facts_all_conditions() const {
  return reached_facts_per_condition;
}

bool Rule::head_is_ground() const {
  return ground_effect;
}
