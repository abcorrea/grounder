#ifndef GROUNDER__ATOM_H_
#define GROUNDER__ATOM_H_

#include "object.h"

#include <cassert>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>


class Arguments {
    std::vector<int> arguments;

public:
    Arguments() = default;

    explicit
    Arguments(std::vector<int> arguments) : arguments(std::move(arguments)) {}

    int operator[] (size_t i) const {
        assert(i < arguments.size());
        return arguments[i];
    }

    std::vector<int>::const_iterator begin() const {
        return arguments.begin();
    }

    std::vector<int>::const_iterator end() const {
        return arguments.end();
    }

    size_t size() const {
        return arguments.size();
    }

    void push_back(int i) {
        arguments.push_back(i);
    };

    void set_value(int i, int j) {
        arguments[i] = j;
    }

};



/*
 *
 * The class Atom represents a any atom of the task. These can be completely
 * lifted atoms, partially ground atoms or even completely ground atoms
 * (although these are usually represented as facts).
 *
 */
class Atom {
    Arguments arguments;
    int predicate_index;
    int index;
    static int next_index;

public:
    Atom(Arguments arguments, int predicate_index) :
        arguments(Arguments(std::move(arguments))),
        predicate_index(predicate_index),
        index(next_index++) {}

    // Print atom in a nice way.
    void print_atom(
        const std::vector<Object> &obj,
        const std::unordered_map<int,std::string> &map_index_to_atom) const;

    const Arguments &get_arguments() const {
        return arguments;
    }

    int get_predicate_index() const {
        return predicate_index;
    }

    int get_index() const {
        return index;
    }

    int argument(size_t i) const {
        assert(i < arguments.size());
        return arguments[i];
    }

};

#endif //GROUNDER__ATOM_H_
