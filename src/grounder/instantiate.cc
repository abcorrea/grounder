#include "instantiate.h"

using namespace std;

void Instantiator::parse(LogicProgram &lp) {
    ifstream infile("actions.out"); // TODO Currently hard-coded, make it a parameter
    int number_of_actions;
    string flag;
    infile >> number_of_actions;
    for (int i = 0; i < number_of_actions; ++i) {
        string name;
        int n_parameters;
        infile >> name >> n_parameters;

        auto action_predicate_idx = lp.get_index_of_atom(name);

        MapParameters map_parameter_to_precond(n_parameters);
        MapParameters map_parameter_to_effect(n_parameters);

        unordered_map<string, int> map_parameter;
        for (int p = 0; p < n_parameters; ++p) {
            int idx;
            string param_name;
            infile >> idx >>  param_name;
            map_parameter.emplace(param_name, idx);
        }
        vector<Atom> precondition;
        vector<Atom> effect;
        parse_conjunctive_formula(lp,infile,flag, map_parameter_to_precond,
                                  map_parameter,precondition);
        parse_conjunctive_formula(lp,infile,flag, map_parameter_to_effect,
                                  map_parameter,effect);

        // TODO what to do with the goal???

        actions.emplace_back(move(precondition),
            move(effect),
            move(map_parameter_to_precond),
            move(map_parameter_to_effect),
            action_predicate_idx);
        map_action_predicate_to_action_schema.emplace(action_predicate_idx, i);

    }
}

void Instantiator::parse_conjunctive_formula(const LogicProgram &lp,
                               ifstream &infile,
                               string &flag,
                               MapParameters &map_parameter_to_atom_position,
                               unordered_map<string, int> &map_parameter_to_index,
                               vector<Atom> &conjunction) const {
    int number_conjunctives;
    infile >> flag >> number_conjunctives;
    for (int pre = 0; pre < number_conjunctives; ++pre) {
        int idx;
        string atom;
        string parameter_name;
        int type;
        int number_param;
        int parameter_index;
        infile >> idx >> atom >> number_param;
        int conjunctive_predicate_idx = lp.get_index_of_atom(atom);
        vector<pair<int, int>> parameters;
        parameters.reserve(number_param);
        for (int t = 0; t < number_param; ++t) {
            infile >> parameter_name;
            if (parameter_name.at(0) == '?') {
                type = VARIABLE;
                parameter_index = map_parameter_to_index.at(parameter_name);
                map_parameter_to_atom_position[parameter_index].emplace_back(pre, t);
            }
            else {
                type = OBJECT;
                parameter_index = lp.get_index_of_object(parameter_name);
            }
            parameters.emplace_back(parameter_index, type);
        }
        Arguments args(parameters);
        conjunction.emplace_back(args, conjunctive_predicate_idx);
    }
}