#ifndef GROUNDER__ATOM_H_
#define GROUNDER__ATOM_H_

#include <string>
#include <utility>
#include <vector>


class Atom {
 public:
  Atom(std::vector<int>  arguments,  std::string predicate) :
      arguments (std::move(arguments)),
      predicate(std::move(predicate)) {
    index = next_index++;
  }

  static int next_index;

  std::vector<int> arguments;
  std::string predicate;
  int index;

 private:

};


#endif //GROUNDER__ATOM_H_
