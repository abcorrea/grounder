#ifndef GROUNDER__ATOM_H_
#define GROUNDER__ATOM_H_

#include "object.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>

/*
 *
 * The class Atom represents a any atom of the task. These can be completely
 * lifted atoms, partially ground atoms or even completely ground atoms
 * (although these are usually represented as facts).
 *
 */
class Atom {
 public:
  Atom(std::vector<int>  arguments,
      std::string  predicate,
      int predicate_index) :
      arguments (std::move(arguments)),
      predicate(std::move(predicate)),
      predicate_index(predicate_index) {
    index = next_index++;
  }


  // Print atom in a nice way.
  void print_atom(const std::vector<Object> &obj) const {
    std::cout << predicate << '(';
    int cont = 0;
    for (int a : arguments) {
      if (a >= 0) {
        // a >= 0 --> object. Print its name
        std::cout << obj[a].predicate;
      } else {
        // a < 0 --> free variable. Print the free variable of the rule.
        std::cout << '?' << char(65 + a);
      }
      cont++;
      if (cont != arguments.size())
        std::cout << ", ";
    }
    std::cout << ")." << std::endl;
  }


  std::vector<int> arguments;
  std::string predicate;
  int predicate_index;
  int index;

 private:
  static int next_index;
};


#endif //GROUNDER__ATOM_H_
