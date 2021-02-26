#ifndef GROUNDER__INSTANTIATE_H_
#define GROUNDER__INSTANTIATE_H_

#include "logic_program.h"

#include <iostream>
#include <fstream>
#include <sstream>

// Maps to a list of pairs <C, P>, where C is the position of the atom in the conjunction
// (i.e., the atom in the effect or precondition in which this parameter appears) and
// P is the position inside the atom (e.g., third free variable)
using MapParameters = std::vector<std::vector<std::pair<int,int>>>;

class ActionSchema {
    std::vector<Atom> precondition;
    std::vector<Atom> effect;
    MapParameters map_parameter_to_precond;
    MapParameters map_parameter_to_effect;
    int pred_idx;

public:
    ActionSchema(
        std::vector<Atom> &&p, std::vector<Atom> &&e,
        MapParameters &&mp, MapParameters &&me, int idx) :
        precondition(std::move(p)), effect(std::move(e)),
        map_parameter_to_precond(std::move(mp)), map_parameter_to_effect(std::move(me)), pred_idx(idx)
    {}

    const std::vector<Atom>& get_precondition() {
        return precondition;
    }

    const std::vector<Atom>& get_effect() {
        return effect;
    }

    std::vector<std::pair<int,int>> get_preconditions_with_variable(int idx) {
        return map_parameter_to_precond[idx];
    }

    std::vector<std::pair<int,int>> get_effects_with_variable(int idx) {
        return map_parameter_to_effect[idx];
    }

    void replace_precondition_variable(int atom_position, int free_var_position, int object) {
        precondition[atom_position].update_value_of_argument(free_var_position, object);
    }

    void replace_effect_variable(int atom_position, int free_var_position, int object) {
        effect[atom_position].update_value_of_argument(free_var_position, object);
    }

};

class Instantiator {
    std::vector<ActionSchema> actions;
    std::unordered_map<int, int> map_action_predicate_to_action_schema;

    void parse(LogicProgram &lp);

    void parse_conjunctive_formula(const LogicProgram &lp,
                                   std::ifstream &infile,
                                   std::string &flag,
                                   MapParameters &map_parameter_to_atom_position,
                                   std::unordered_map<std::string, int> &map_parameter_to_index,
                                   std::vector<Atom> &conjunction) const;

public:
    Instantiator() = default;

    void instantiate_actions(LogicProgram &lp, const std::set<int>& ground_actions_idx) {
        parse(lp);

        for (const int idx : ground_actions_idx) {
            const Fact& action = lp.get_fact_by_index(idx);
            action.print_atom(lp.get_objects(), lp.get_map_index_to_atom());
            // TODO add assertion
            int action_schema_idx = map_action_predicate_to_action_schema.at(action.get_predicate_index());

            ActionSchema action_schema = actions[action_schema_idx]; // copy
            int count = 0;
            for (Term arg : action.get_arguments()) {
                assert(arg.is_object());
                for (auto precond_pair : action_schema.get_preconditions_with_variable(count)) {
                    int atom_pos = precond_pair.first;
                    int argument_pos = precond_pair.second;
                    action_schema.replace_precondition_variable(atom_pos, argument_pos, arg.get_index());
                }
                for (auto effect_pair : action_schema.get_effects_with_variable(count)) {
                    int atom_pos = effect_pair.first;
                    int argument_pos = effect_pair.second;
                    action_schema.replace_effect_variable(atom_pos, argument_pos, arg.get_index());
                }
                ++count;
            }
            std::cout << "Precondition: ";
            for (const Atom &p : action_schema.get_precondition()) {
                if (p.is_negated()) {
                    std::cout << "not ";
                }
                p.print_atom_no_newline(lp.get_objects(), lp.get_map_index_to_atom());
            }
            std::cout << std::endl;
            std::cout << "Effect: ";
            for (const Atom &e : action_schema.get_effect()) {
                if (e.is_negated()) {
                    std::cout << "not ";
                }
                e.print_atom_no_newline(lp.get_objects(), lp.get_map_index_to_atom());
            }
            std::cout << std::endl << std::endl;
        }
        std::cout << "Finishing instantiation" << std::endl;
    }

};

#endif //GROUNDER__INSTANTIATE_H_
