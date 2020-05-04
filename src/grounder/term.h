#ifndef GROUNDER_TERM_H
#define GROUNDER_TERM_H

#include <boost/functional/hash.hpp>

enum TERM_TYPES {CONSTANT, VARIABLE};

class Term {
    int index;
    int type;

public:
    Term() = default;

    Term(int i, int t) : index(i), type(t) {}

    int get_index() const {
        return index;
    }

    bool is_constant() const {
        return (type == CONSTANT);
    }

    void set_value(int i) {
        index = i;
        type = CONSTANT;
    }

    friend std::size_t hash_value(const Term &t) {
        std::size_t seed = 0;
        boost::hash_combine(seed, t.get_index());
        boost::hash_combine(seed, t.is_constant());
        return seed;
    }

    friend bool operator==(const Term &lhs, const Term &rhs) {
        return ((rhs.get_index() == rhs.get_index()) and (lhs.is_constant() & rhs.is_constant()));
    }

};


#endif //GROUNDER_TERM_H