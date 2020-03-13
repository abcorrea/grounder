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
 protected:
  static int next_index;

 public:
  explicit Rule(Atom effect, std::vector<Atom> c, int type) :
      effect(std::move(effect)), conditions(std::move(c)), type(type) {
    index = next_index++;
    hash_table_indices.clear();
    reached_facts_per_condition.clear();
    if (type == JOIN) {
      hash_table_indices.resize(2);
      matches = computing_matching_variables();
    } else if (type == PRODUCT) {
      reached_facts_per_condition.resize(conditions.size());
      for (int i = 0; i < conditions.size(); ++i)
        reached_facts_per_condition[i].push_back(std::vector<int>());
    }
  };

  Atom effect;
  std::vector<Atom> conditions;
  int index;
  int type;
  std::vector<std::unordered_map<std::vector<int>, std::unordered_set<Fact>,
                                 boost::hash<std::vector<int>>>>
      hash_table_indices;

  // Only need to keep track of this for product rules, the other ones are very
  // predictable and have a well-behaved structure
  std::vector<std::vector<std::vector<int>>>
      reached_facts_per_condition;

  // A vector with two elements indicating the positions in which the variables
  // in the key occur in each respective rule of the body. If the first element
  // has X in it's 3rd position, than it means that the first rule of the body
  // has the third variable of the key in its Xth position.
  std::vector<std::vector<int>> position_of_matching_vars;
  std::vector<int> matches;

 private:
  std::vector<int> computing_matching_variables();
};

#endif //GROUNDER__RULES_H_
