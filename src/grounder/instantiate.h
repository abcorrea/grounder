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
        std::cout << "Finishing instantiation" << std::endl;
    }

};

#endif //GROUNDER__INSTANTIATE_H_
