#ifndef GROUNDER_GROUNDERS_NEW_GROUNDER_H_
#define GROUNDER_GROUNDERS_NEW_GROUNDER_H_

#include "fast_downward_grounder.h"

#include "../utils/scc.h"

#include <iostream>

class NewGrounder : public FastDownwardGrounder  {
public:
    NewGrounder() {};

    int ground(LogicProgram &lp) override {
        DirectedGraph dependency_graph;
        for (const auto &rule : lp.get_rules()) {
            dependency_graph.add_node(rule->get_effect().get_predicate_index());
            for (const auto &cond1 : rule->get_conditions()) {
                dependency_graph.add_node(cond1.get_predicate_index());
                dependency_graph.add_edge(cond1.get_predicate_index(),
                                          rule->get_effect().get_predicate_index(),
                                          rule->get_index());
            }
        }

        SCC scc(dependency_graph);

        return FastDownwardGrounder::ground(lp);
    }

};

#endif //GROUNDER_GROUNDERS_NEW_GROUNDER_H_
