#ifndef GROUNDER_HASH_TABLE_H
#define GROUNDER_HASH_TABLE_H

#include "fact.h"

#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <boost/functional/hash.hpp>

typedef std::vector<int> JoinHashKey;

class JoinHashEntry {
    std::unordered_set<Fact> entry;

public:
    JoinHashEntry() = default;

    void insert(const Fact &f) {
        entry.insert(f);
    }

    std::unordered_set<Fact>::const_iterator begin() const {
        return entry.begin();
    }

    std::unordered_set<Fact>::const_iterator end() const {
        return entry.end();
    }

};

class JoinHashTable {
    std::unordered_map<JoinHashKey, JoinHashEntry, boost::hash<JoinHashKey>> hash_table_1;
    std::unordered_map<JoinHashKey, JoinHashEntry, boost::hash<JoinHashKey>> hash_table_2;

    static bool valid_position(size_t i) {
        return (i == 0 or i == 1);
    }

public:
    JoinHashTable() = default;

    void insert(const Fact &f, const JoinHashKey &key, int position) {
        assert (valid_position(position));
        if (position == 0) {
            hash_table_1.emplace(key, JoinHashEntry());
            hash_table_1[key].insert(f);
        }
        else {
            hash_table_2.emplace(key, JoinHashEntry());
            hash_table_2[key].insert(f);
        }
    }

    const JoinHashEntry &get_entries(const JoinHashKey &key, size_t position) {
        assert(valid_position(position));
        if (position == 0)
            return hash_table_1[key];
        else
            return hash_table_2[key];
    }

};

#endif //GROUNDER__HASH_TABLE_H
