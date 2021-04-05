#include "instantiate.h"
#include "logic_program.h"
#include "parser.h"

#include "grounders/grounder_factory.h"
#include "utils/system.h"

#include <ctime>
#include <iostream>

using namespace std;

void instantiate_actions(LogicProgram program, set<int> set);

int main(int argc, char *argv[]) {
    std::cout << "Initializing grounding!" << std::endl;

    const bool instantiate = true; // TODO make this a parameter

    if (argc!=3) {
        cerr << "Usage: ./grounder [LP PROGRAM] [GROUNDING METHOD]" << endl;
        exit(-1);
    }
    clock_t timer_start = clock();
    cout << "Opening file: " << argv[1] << endl;
    ifstream in(argv[1]);
    if (!in) {
        cerr << "Error opening the task file." << endl;
        exit(-1);
    }

    LogicProgram logic_program = parse(in);

    double parsing_time = double(clock() - timer_start)/CLOCKS_PER_SEC;
    cout << "Parsing time: " << parsing_time << "s" << endl;

    cout << "Number of initial facts: " << logic_program.get_facts().size() << endl;
    cout << "Number of rules: " << logic_program.get_rules().size() << endl;

    Grounder *grounder = GrounderFactory::new_grounder(argv[2]);

    cout << "Starting to ground the logical program..." << endl;

    int grounded = grounder->ground(logic_program);
    if (grounded!=0) {
        cerr << "Error during grounding procedure. Grounding not finished!"
             << endl;
    }
    double total_time = double(clock() - timer_start)/CLOCKS_PER_SEC;
    double grounding_time = total_time - parsing_time;
    cout << "Grounding time: " << grounding_time << "s" << endl;
    cout << "Total time: " << total_time << "s" << endl;
    cout << "Peak memory usage: " << utils::get_peak_memory_in_kb() << " kB\n";

    //const auto &map_index_to_atom = logic_program.get_map_index_to_atom();
    int number_of_ground_actions = 0;
    int counter = 0;
    set<int> ground_action_idx;
    set<int> fluents_idx;
    for (const auto &f : logic_program.get_facts()) {
      if (logic_program.is_action_predicate(f.get_predicate_index())) {
          ground_action_idx.emplace(counter);
          ++number_of_ground_actions;
      } else {
          if (logic_program.get_atom_by_index(f.get_predicate_index()).rfind("p$", 0) != 0) {
              // If not action nor auxiliary predicate
              fluents_idx.emplace(f.get_fact_index());
          }
      }
      //f.print_atom(logic_program.get_objects(), logic_program.get_map_index_to_atom());
      ++counter;
    }

    cout << "Number of ground actions: " << number_of_ground_actions << endl;
    cout << logic_program.get_facts().size() << " atoms" << endl;

    if (instantiate) {
        Instantiator instantiator;
        instantiator.instantiate_actions(logic_program, ground_action_idx);
    }

    return 0;
}

