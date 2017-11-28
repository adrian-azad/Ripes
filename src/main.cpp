#include <string.h>
#include <iomanip>
#include <iostream>

#include "parser.h"
#include "runner.h"

using namespace std;

int main(int argc, char **argv) {
  if (argc != 3) {
    // insert error msg
    cout << "Invalid number of arguments\n" << endl;
    cout << "Usage: $appname $binaryname $hex/bin" << endl;
    cout << "hex: onscreen hexdump, bin: binary dump to file simdump.bin"
         << endl;
    return 1;
  }

  Parser parser;
  // For now, initialize parser with input argument
  if (parser.init(argv[1])) {
    cout << "File could not be loaded\n" << endl;
    return 1;
  }

  Runner runner(&parser);
  if (!runner.exec()) {
    auto regPtr = runner.getRegPtr();
    if (strcmp(argv[2], "hex") == 0) {
      // output hexdump of registers
      cout << "Register contents after simulating: " << argv[1] << endl;
      for (int i = 0; i < 32; i++) {
        cout << "x(" << std::dec << i << "): " << std::hex << setw(20)
             << (*regPtr)[i] << endl;
      }
    } else if (strcmp(argv[2], "bin") == 0) {
      // binary dump
      ofstream dumpFile;
      char value[4];
      dumpFile.open("simdump.bin", ios::binary | ios::out);
      /* reversed order
      for (int i = 0; i < 32; i++) {
        uint32_t regValue = (*regPtr)[i];
        for (int j = 0; j < 4; j++) {
          value[j] = regValue >> 24;
          regValue <<= 8;
        }
        dumpFile.write(value, 4);
      }
      */
      for (int i = 0; i < 32; i++) {
        uint32_t regValue = (*regPtr)[i];
        for (int j = 0; j < 4; j++) {
          value[j] = regValue & 0xff;
          regValue >>= 8;
        }
        dumpFile.write(value, 4);
      }
      dumpFile.close();
    }
  }
  return 0;
}
