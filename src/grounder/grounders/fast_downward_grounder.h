#ifndef GROUNDER_GROUNDERS_FAST_DOWNWARD_GROUNDER_H_
#define GROUNDER_GROUNDERS_FAST_DOWNWARD_GROUNDER_H_

#include "grounder.h"

#include "../rule_matcher.h"

#include <iostream>
#include <optional>
#include <unordered_set>

class FastDownwardGrounder : public Grounder {

    RuleMatcher rule_matcher;

    void create_rule_matcher(const LogicProgram &lp) {
        // Loop over rule conditions
        for (const auto &rule : lp.get_rules()) {
            int cont = 0;
            for (const auto &condition : rule.get_conditions()) {
                rule_matcher.insert(condition.get_predicate_index(),
                                    rule.get_index(),
                                    cont++);
            }
        }
    }

    static std::optional<Fact> project(const Rule &rule, const Fact &fact);
    static std::vector<Fact> join(Rule &rule, const Fact &fact, int position);
    static bool is_new(Fact &new_fact,
                       std::unordered_set<Fact> &reached_facts,
                       LogicProgram &lp);
    static std::vector<Fact> product(Rule &rule,
                                     const Fact &fact,
                                     int position);

public:
    FastDownwardGrounder() {
        std::cout << "Using Fast Downward grounding algorithm..." << std::endl;
    }

    int ground(LogicProgram &lp) override;

};

#endif //GROUNDER_GROUNDERS_FAST_DOWNWARD_GROUNDER_H_
