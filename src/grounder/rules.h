#ifndef GROUNDER__RULES_H_
#define GROUNDER__RULES_H_

#include <string>
#include <utility>
#include "atom.h"

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
#define PRODUCT 3

class Rule {
 protected:
  static int next_index;
 public:
  explicit Rule(Atom effect, std::vector<Atom> conditions, int type) :
      effect(std::move(effect)), conditions(std::move(conditions)), type(type) {
    index = next_index++;
    type = -1;
  };

  Atom effect;
  std::vector<Atom> conditions;
  int index;
  int type;
};

#endif //GROUNDER__RULES_H_
