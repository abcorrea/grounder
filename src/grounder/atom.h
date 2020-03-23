#ifndef GROUNDER__ATOM_H_
#define GROUNDER__ATOM_H_

#include "object.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>

/*
 *
 * The class Atom represents a any atom of the task. These can be completely
 * lifted atoms, partially ground atoms or even completely ground atoms
 * (although these are usually represented as facts).
 *
 */
class Atom {
  std::vector<int> arguments;
  int predicate_index;
  int index;

public:
    Atom(std::vector<int>  arguments, int predicate_index) :
        arguments(std::move(arguments)),
        predicate_index(predicate_index),
        index(next_index++)
    { }


  // Print atom in a nice way.
  void print_atom(const std::vector<Object> &obj,
      const std::unordered_map<int, std::string>& map_index_to_atom) const;

  const std::vector<int> &get_arguments() const {
    return arguments;
  }
  int get_predicate_index() const {
    return predicate_index;
  }
  int get_index() const {
    return index;
  }

  const int get_argument_by_index(int i) const {
    return arguments[i];
  }

 protected:
  static int next_index;
};


#endif //GROUNDER__ATOM_H_
