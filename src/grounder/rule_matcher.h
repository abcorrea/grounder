#ifndef GROUNDER__RULE_MATCHER_H_
#define GROUNDER__RULE_MATCHER_H_

#include <unordered_map>
#include <utility>
#include <vector>

class Match {
    int rule;
    int position;

public:
    Match(int r, int p) : rule(r), position(p) {}

    int get_rule() const {
        return rule;
    }
    int get_position() const {
        return position;
    }
};

class Matches {
    std::vector<Match> matches;

public:
    Matches() = default;

    explicit
    Matches(std::vector<Match> &&matches) : matches(std::move(matches)) {}

    void insert_new_match(int r, int p) {
        matches.emplace_back(r, p);
    }

    std::vector<Match>::const_iterator begin() const {
        return matches.begin();
    }

    std::vector<Match>::const_iterator end() const {
        return matches.end();
    }

};

class RuleMatcher {
    /*
     Map index of an atom to a vector rule matches
    */
    std::unordered_map<int, Matches> rule_matcher;

public:
    RuleMatcher() = default;

    RuleMatcher(std::unordered_map<int, Matches> rule_matcher) :
        rule_matcher(std::move(rule_matcher)) {}

    bool atom_has_matched_rules(int i) const {
        return (rule_matcher.find(i)!=rule_matcher.end());
    }

    void insert(int predicate_index, int rule_index, int position) {
        if (!atom_has_matched_rules(predicate_index)) {
            rule_matcher[predicate_index] = Matches();
        }
        rule_matcher[predicate_index].insert_new_match(rule_index, position);
    }

    const Matches &get_matched_rules(int index) const {
        return rule_matcher.at(index);
    }

};

#endif //GROUNDER__RULE_MATCHER_H_
