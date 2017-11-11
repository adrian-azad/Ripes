#include <iomanip>
#include <iostream>

#include "parser.h"
#include "runner.h"

using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    // insert error msg
    cout << "Invalid number of arguments\n" << endl;
    cout << "Usage: $appname $binaryname" << endl;
    return 1;
  }

  Parser parser;
  // For now, initialize parser with input argument
  if (parser.init(argv[1])) {
    return 1;
  }
  Runner runner(&parser);

  if (!runner.exec()) {
    // output hexdump of registers
    cout << "Register contents after simulating: " << argv[1] << endl;
    auto regPtr = runner.getRegPtr();
    for (int i = 0; i < 32; i++) {
      cout << "x(" << std::dec << i << "): " << std::hex << setw(20)
           << (*regPtr)[i] << endl;
    }
  }
  return 0;
}
