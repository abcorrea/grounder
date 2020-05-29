#include "logic_program.h"
#include "parser.h"

#include "grounders/grounder_factory.h"
#include "utils/system.h"

#include <ctime>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    std::cout << "Initializing grounding!" << std::endl;

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

    /*for (const auto &f : logic_program.get_facts())
      f.print_atom(logic_program.get_objects(), logic_program.get_map_index_to_atom());*/
    cout << logic_program.get_facts().size() << " atoms" << endl;


    return 0;
}
