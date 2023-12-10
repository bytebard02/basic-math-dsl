#include <fstream>
#include <iostream>
#include "interpreter.h"
#include "lexer.h"
#include "parser.h"

void printDebugInfo(Lexer& lexer, Parser& parser);
bool DEBUG_MODE = false;

int main(int argc, char* argv[]) {
  while (true) {
    system("clear");
    std::string input;
    bool isFromFile = false;

    if (argc > 1) {
      std::ifstream file(argv[1]);
      if (file) {
        std::string line;
        while (std::getline(file, line)) {
          input += line + "\n";
        }
        isFromFile = true;
        system("clear");
        std::cout << input << std::endl << std::endl;
      } else {
        std::cerr << "Failed to open file: " << argv[1] << std::endl;
        return 1;
      }
    } else {
      std::string line;
      while (std::getline(std::cin, line)) {
        input += line + "\n";
        if (line == "run") {
          break;
        }
      }
    }

    try {
      Lexer lexer(input);
      lexer.tokenize();

      Parser parser(lexer);
      auto ast = parser.parse();

      if (DEBUG_MODE) {
        printDebugInfo(lexer, parser);
      }

      Interpreter interpreter;
      interpreter.interpret(ast);

    } catch (const IllegalCharError& e) {
      std::cerr << e.asString() << std::endl;
    } /*  catch (...) {
       std::cerr << "Caught an unexpected exception!" << std::endl;
     } */
    if (isFromFile) {
      break;  // If the input was from a file, break the loop
    } else {
      std::cout << "\n> Press 'Enter' to clear your code.\n";
      char c;
      std::cin.get(c);
      if (c == '\n') {
        continue;
      } else {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                        '\n');  // Ignore the rest of the line
      }
    }
  }

  return 0;
}