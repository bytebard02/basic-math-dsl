#pragma once
#include <iostream>
#include <memory>
#include <string>
#include "AST.h"
#include "error.h"

class Interpreter {
  std::unordered_map<std::string, std::optional<int>> symbolTable;

 public:
  Interpreter();
  int interpret(std::shared_ptr<ASTNode> root);

 private:
  void executeStatement(std::shared_ptr<ASTNode> node);
  void executeVarDeclaration(std::shared_ptr<ASTNode> node);
  void executeAssignment(std::shared_ptr<ASTNode> node);
  void executeIfStatement(std::shared_ptr<ASTNode> node);
  void executeWhileStatement(std::shared_ptr<ASTNode> node);
  void executeStatementList(std::shared_ptr<ASTNode> node);
  bool evaluateCondition(std::shared_ptr<ASTNode> node);
  int visit(std::shared_ptr<ASTNode> node);
  int visitExpression(std::shared_ptr<ASTNode> node);
  int visitIdentifier(std::shared_ptr<ASTNode> node);
  int visitTerm(std::shared_ptr<ASTNode> node);
  int visitFactor(std::shared_ptr<ASTNode> node);
  int visitLiteral(std::shared_ptr<ASTNode> node);
  int visitUnaryMinus(std::shared_ptr<ASTNode> node);
  int visitPrintStatement(std::shared_ptr<ASTNode> node);
  int visitOperator(std::shared_ptr<ASTNode> node);
};
