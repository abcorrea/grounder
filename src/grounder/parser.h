#ifndef GROUNDER__PARSER_H_
#define GROUNDER__PARSER_H_

#include <fstream>
#include <iostream>
#include <unordered_map>

#include "logic_program.h"

bool parse(LogicProgram &lp, std::ifstream &in);

bool is_warning_message(const std::string& line);

std::string get_atom_name(const std::string &str);

std::vector<std::string> extract_arguments_from_atom(const std::string &atom);

std::vector<int> transform_args_into_indices(
    std::unordered_map<std::string, int> &map_objects,
    std::unordered_map<std::string, int> &map_variables,
    const std::vector<std::string> &arguments,
    std::vector<Object> &lp_objects,
    int &number_of_vars_current_rule);

std::vector<std::string> get_rule_conditions(std::string &body);


#endif //GROUNDER__PARSER_H_
