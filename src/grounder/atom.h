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
public:
    Atom(std::vector<int>  arguments, int predicate_index) :
        arguments(std::move(arguments)),
        predicate_index(predicate_index),
        index(next_index++)
    { }


  // Print atom in a nice way.
  void print_atom(const std::vector<Object> &obj,
      const std::unordered_map<int, std::string>& map_index_to_atom) const {
    std::cout << map_index_to_atom.at(predicate_index) << '(';
    size_t cont = 0;
    for (int a : arguments) {
      if (a >= 0) {
        // a >= 0 --> object. Print its name
        std::cout << obj[a].name;
      } else {
        // a < 0 --> free variable. Print the free variable of the rule.
        std::cout << '?' << char(65 + a);
      }
      cont++;
      if (cont != arguments.size())
        std::cout << ", ";
    }
    std::cout << ')' << std::endl;
  }


  std::vector<int> arguments;
  int predicate_index;
  int index;

protected:
    static int next_index;
};


#endif //GROUNDER__ATOM_H_
