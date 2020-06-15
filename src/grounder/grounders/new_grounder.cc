#include "new_grounder.h"

#include <algorithm>
#include <iostream>
#include <queue>

using namespace std;

void NewGrounder::dump_sccs(const LogicProgram &lp, const SCC &scc) const {
    int i = 0;
    for (const auto &c : scc.get_components()) {
        cout << "Component #" << i++ << ": ";
        for (auto i : c) {
            cout << lp.get_atom_by_index(i) << " (" << i << ") ";
        }
        cout << endl;
    }
}

int NewGrounder::ground(LogicProgram &lp) {
    {
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

        SCC sccs(dependency_graph);
        //dump_sccs(lp, sccs);
        cout << "Number of SCCs: " << sccs.size() << endl;
        int component_counter = 0;
        for (const auto &c : sccs.get_components()) {
            for (int e : c) {
                map_pred_to_scc[e] = component_counter;
            }
            ++component_counter;
        }

        create_rule_matcher(lp);

        unordered_set<Fact> reached_facts;

        get_useful_rules_per_component(lp, sccs);

        for (const Fact &f : lp.get_facts()) {
            do_insertion(map_pred_to_scc[f.get_predicate_index()], f.get_fact_index());
            reached_facts.insert(f);
        }

        int last_component = 0;
        while (q_size > 0) {
            auto min_element = remove_min();
            int component = min_element.first;
            int i = min_element.second;
            if (component > last_component) {
                for (int r : rule_delete_component[last_component]) {
                    //cerr << last_component << ' ' << r << endl;
                    lp.clean_rule(r);
                }
                last_component = component;
            }
            Fact current_fact = lp.get_fact_by_index(i);
            int predicate_index = current_fact.get_predicate_index();
            for (const auto
                    &m : rule_matcher.get_matched_rules(predicate_index)) {
                int rule_index = m.get_rule();
                int position_in_the_body = m.get_position();
                RuleBase &rule = lp.get_rule_by_index(rule_index);

                if (rule.get_type()==PROJECT) {
                    // Projection rule - single condition in the body
                    assert(position_in_the_body==0);
                    optional<Fact> new_fact = project(rule, current_fact);
                    if (new_fact and lp.is_new(*new_fact, reached_facts)) {
                        do_insertion(map_pred_to_scc[new_fact->get_predicate_index()],
                                  new_fact->get_fact_index());
                    }
                } else if (rule.get_type()==JOIN) {
                    // Join rule - two conditions in the body
                    assert(position_in_the_body <= 1);
                    for (Fact new_fact : join(rule,
                                              current_fact,
                                              position_in_the_body))
                        if (lp.is_new(new_fact, reached_facts)) {
                            do_insertion(map_pred_to_scc[new_fact.get_predicate_index()],
                                      new_fact.get_fact_index());
                        }
                } else if (rule.get_type()==PRODUCT) {
                    // Product rule - more than one condition without shared free vars
                    for (Fact new_fact : product(rule, current_fact, position_in_the_body))
                        if (lp.is_new(new_fact, reached_facts)) {
                            do_insertion(map_pred_to_scc[new_fact.get_predicate_index()],
                                      new_fact.get_fact_index());
                        }
                }
            }
        }
        return 0;
    }
}


void NewGrounder::get_useful_rules_per_component(const LogicProgram &lp, const SCC &sccs) {
    rule_delete_component.resize(sccs.get_components().size());
    for (const auto &rule : lp.get_rules()) {
        int effect_index = rule->get_effect().get_predicate_index();
        int scc_of_effect = map_pred_to_scc[effect_index];
        int rule_index = rule->get_index();
        rule_delete_component[scc_of_effect].push_back(rule_index);
    }
}
