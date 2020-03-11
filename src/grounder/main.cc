#include <fstream>
#include <iostream>

#include "logic_program.h"
#include "parser.h"

#include "grounders/grounder_factory.h"

using namespace std;

int main(int argc, char *argv[]) {
  std::cout << "Initializing grounding!" << std::endl;

  if (argc != 3) {
    cerr << "Usage: ./grounder [LP PROGRAM] [GROUNDING METHOD]" << endl;
    exit(-1);
  }

  cout << "Opening file: " << argv[1] << endl;
  ifstream in(argv[1]);
  if (!in) {
    cerr << "Error opening the task file." << endl;
    exit(-1);
  }

  LogicProgram logic_program;

  bool parsed = parse(logic_program, in);
  if (!parsed) {
    cerr << "Parser failed." << endl;
    return -1;
  }

  logic_program.create_rule_matcher();

  Grounder *grounder = GrounderFactory::new_grounder(argv[2]);

  cout << "Starting to ground the logical program..." << endl;
  int grounded = grounder->ground(logic_program);
  if (grounded != 0) {
    cerr << "Error during grounding procedure. Grounding not finished!" << endl;
  }

  return 0;
}
