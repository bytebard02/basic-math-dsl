#pragma once
#include <iostream>
#include <stack>
#include <vector>
#include "AST.h"
#include "lexer.h"

class Parser {
 public:
  Parser(Lexer& lexer);
  std::shared_ptr<ASTNode> parse();
  std::shared_ptr<ASTNode> getAST();

 private:
  Lexer lexer;
  Token currentToken;
  Token nextToken;

  std::shared_ptr<ASTNode> parseStatement(int indentLevel = 0);
  std::shared_ptr<ASTNode> parseExpression();
  std::shared_ptr<ASTNode> parseTerm();
  std::shared_ptr<ASTNode> parseFactor();
  std::shared_ptr<ASTNode> parsePrintStatement();
  std::shared_ptr<ASTNode> parseVarDeclaration();
  std::shared_ptr<ASTNode> parseAssignment();
  std::shared_ptr<ASTNode> parseDataType();
  std::shared_ptr<ASTNode> parseIdentifier();
  std::shared_ptr<ASTNode> parseIfStatement(int indentLevel);
  std::shared_ptr<ASTNode> parseWhileStatement(int indentLevel);
  std::shared_ptr<ASTNode> parseComparison();
  std::shared_ptr<ASTNode> parseElifStatement(int indentLevel);
  std::shared_ptr<ASTNode> parseElseStatement(int indentLevel);
  std::shared_ptr<ASTNode> parseComparator();
  std::shared_ptr<ASTNode> parseIndentedStatementList(int indentLevel);
  void advance();
  void eat(TokenType type);

  std::shared_ptr<ASTNode> root;
};
