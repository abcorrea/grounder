#ifndef GROUNDER__FACT_H_
#define GROUNDER__FACT_H_

#include <string>
#include <utility>
#include <vector>

#include "atom.h"

/*
 *
 * The class fact represents a ground atom that is true in the LP.
 * The arguments of the atom are represented by their index, and they refer
 * to Objects. Each Fact also has an index and a predicate name.
 *
 */
class Fact : public Atom {
 public:
  Fact(std::vector<int> arguments,
       std::string predicate,
       int predicate_index) : Atom(std::move(arguments),
                                   std::move(predicate),
                                   predicate_index) {}

 private:

};

#endif //GROUNDER__FACT_H_
