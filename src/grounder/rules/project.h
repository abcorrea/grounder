#ifndef GROUNDER_RULES_PROJECT_RULE_H
#define GROUNDER_RULES_PROJECT_RULE_H

#include "rule_base.h"

class ProjectRule : public RuleBase {
public:
    using RuleBase::RuleBase;

    int get_type() const override {
        return PROJECT;
    }
};


#endif //GROUNDER_RULES_PROJECT_RULE_H
