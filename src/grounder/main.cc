#include <fstream>
#include <iostream>

#include "logic_program.h"
#include "parser.h"

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

  return 0;
}
