#ifndef GROUNDER_RULES_PROJECT_RULE_H
#define GROUNDER_RULES_PROJECT_RULE_H

#include "rule_base.h"

class ProjectRule : public RuleBase {
public:
    using RuleBase::RuleBase;

    int get_type() const override {
        return PROJECT;
    }

    const Arguments &get_condition_arguments() const {
        return conditions[0].get_arguments();
    }
};


#endif //GROUNDER_RULES_PROJECT_RULE_H
