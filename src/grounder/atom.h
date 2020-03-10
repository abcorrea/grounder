#ifndef GROUNDER__ATOM_H_
#define GROUNDER__ATOM_H_

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

  static int next_index;

  std::vector<int> arguments;
  std::string predicate;
  int predicate_index;
  int index;

 private:

};


#endif //GROUNDER__ATOM_H_
