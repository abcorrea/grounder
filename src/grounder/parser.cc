#include "parser.h"

#include <fstream>
#include <sstream>
#include <string>

#include <boost/algorithm/string.hpp>

using namespace std;

int number_of_facts = 0;
int number_of_rules = 0;
int number_of_objects = 0;

bool parse(LogicProgram &lp, ifstream &in) {
  cout << "Parsing file..." << endl;

  unordered_map<string, int> map_object_to_index;

  vector<Object> lp_objects;
  vector<Fact> lp_facts;
  vector<Rule> rules;

  string line;

  while (getline(in, line)) {
    // Check if it is a fact or a rule
    if (line.find(":-") != string::npos) {
      // Rule
      int number_of_vars_current_rule = -1; // Variables have negative counter

      string head = line.substr(0, line.find(':'));
      string body = line.substr(line.find(':')); // Still contains ':-'

      string rule_type = head.substr(0, head.find(' '));
      string head_atom_name = head.substr(head.find(' '));
      string head_predicate = get_atom_name(head_atom_name);
      vector<string>
          head_arguments = extract_arguments_from_atom(head_atom_name);

      unordered_map<string, int> map_variables_to_index;
      vector<int> head_argument_indices = transform_args_into_indices(
          map_object_to_index,
          map_variables_to_index,
          head_arguments,
          lp_objects,
          number_of_vars_current_rule);

      Atom head_atom(head_argument_indices, head_atom_name);

      vector<string> condition_atoms_strings = get_rule_conditions(body);
      vector<Atom> condition_atoms;
      condition_atoms.reserve(condition_atoms_strings.size());
      for (auto s : condition_atoms_strings) {
        string atom_name = get_atom_name(s);
        vector<string> atom_arguments = extract_arguments_from_atom(s);
        vector<int> indices = transform_args_into_indices(
          map_object_to_index,
          map_variables_to_index,
          atom_arguments,
          lp_objects,
          number_of_vars_current_rule);
        condition_atoms.emplace_back(indices, atom_name);
      }

      if (boost::iequals(rule_type, "project")) {
        // Project rule
        rules.emplace_back(head_atom, condition_atoms, PROJECT);
      } else if (boost::iequals(rule_type, "join")) {
        // Join rule
        rules.emplace_back(head_atom, condition_atoms, JOIN);
      } else if (boost::iequals(rule_type, "product")) {
        // Product rule
        rules.emplace_back(head_atom, condition_atoms, PRODUCT);
      }

      number_of_rules++;
    } else {
      // Fact
      string predicate = get_atom_name(line);
      vector<string> arguments = extract_arguments_from_atom(line);

      vector<int> arguments_indices;
      for (const auto &argument : arguments) {
        auto it_pair =
            map_object_to_index.try_emplace(argument, number_of_objects);
        if (it_pair.second) {
          // Object is new to the map. Increase counter.
          lp_objects.emplace_back(argument);
          number_of_objects++;
        }
        arguments_indices.push_back(map_object_to_index[argument]);
      }

      lp_facts.emplace_back(arguments_indices, predicate);
      number_of_facts++;
    }
  }

  return true;
}

/*
 * Get atom name
 */
string get_atom_name(const string &str) {
  return boost::trim_copy(str.substr(0, str.find('(')));
}

/*
 * Extract the arguments (i.e., parameters) of atoms. The atom can be anything
 * in the format X(a, b, c, ..., z) where X is a string and a, b, c, ..., z are
 * also strings separated by comma.
 */
vector<string> extract_arguments_from_atom(const string &atom) {
  unsigned args_start = atom.find('(');
  unsigned args_end = atom.find(')');
  string arguments_in_str = boost::trim_copy(atom.substr(args_start, args_end));
  arguments_in_str.erase(0, 1); // Removes first delimiter, '('
  if (arguments_in_str.back() == '.')
    arguments_in_str.pop_back(); // Remove last delimiter, '.'
  if (arguments_in_str.back() == ')')
    arguments_in_str.pop_back(); // Remove last delimiter, ')'
  boost::erase_all(arguments_in_str, " "); // Remove white spaces

  vector<string> arguments;
  boost::split(arguments, arguments_in_str, boost::is_any_of(","));
  return arguments;
}

/*
 * Transforms a vector of arguments (having constants and variables) into a vector
 * of integers. Variables have negative value.
 */
vector<int> transform_args_into_indices(
    unordered_map<string, int> &map_objects,
    unordered_map<string, int> &map_variables,
    const vector<string> &arguments,
    vector<Object> &lp_objects,
    int &number_of_vars_current_rule) {
  vector<int> indices(arguments.size(), 0);
  int counter = 0;
  for (const auto &a : arguments) {
    if (a.front() == '?') {
      // Variable
      auto it_pair = map_variables.try_emplace(a, number_of_vars_current_rule);
      if  (it_pair.second) {
        // Variable is not new to the map. Increase counter.
        number_of_vars_current_rule--;
      }
      indices[counter++] = map_variables[a];
    } else {
      auto it_pair = map_objects.try_emplace(a, number_of_objects);
      if  (it_pair.second) {
        // Object is not new to the map. Increase counter.
        lp_objects.emplace_back(a);
        number_of_objects++;
      }
      indices[counter++] = map_objects[a];
    }
  }
  return indices;
}

vector<string> get_rule_conditions(string &body) {
  vector<string> condition_atoms;
  boost::erase_all(body, ":- ");
  boost::split(condition_atoms, body, boost::is_any_of(")"));

  condition_atoms.pop_back(); // Last one is always a lonely '.'

  return condition_atoms;
}