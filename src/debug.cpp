#include <iostream>
#include <vector>
#include "AST.h"
#include "lexer.h"
#include "parser.h"

void printDebugInfo(Lexer& lexer, Parser& parser) {
  std::cout << "\nDebug info:";

  std::vector<Token> tokens = lexer.getAllTokens();
  std::cout << "\nTokens:\n";
  for (const Token& token : tokens) {
    std::cout << token.asString() << std::endl;
  }

  std::shared_ptr<ASTNode> root = parser.getAST();
  std::cout << "\nAST:\n";
  std::cout << root->asString(0) << std::endl;
}