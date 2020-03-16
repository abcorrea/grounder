#ifndef GROUNDER__OBJECT_H_
#define GROUNDER__OBJECT_H_

#include <string>
#include <utility>
#include <vector>

/*
 * This class represents an object (i.e., constant) of the task and the logic
 * program. Every object has a name and a unique index.
 *
 */
class Object {
 protected:
  static int next_index;
 public:

  explicit Object(std::string name) : name(std::move(name)) {
    index = next_index++;
  };

  std::string name;
  int index = -1;

 private:

};

#endif //GROUNDER__OBJECT_H_
