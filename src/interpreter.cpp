#include "interpreter.h"

Interpreter::Interpreter() {}

int Interpreter::interpret(std::shared_ptr<ASTNode> root) {
  if (root && root->type == NodeType::Program) {
    for (const auto& child : root->children) {
      executeStatement(child);
    }
  } else {
    throw std::runtime_error("Invalid AST");
  }
  return 0;
}

int Interpreter::visit(std::shared_ptr<ASTNode> node) {
  switch (node->type) {
    case NodeType::Identifier:
      return visitIdentifier(node);
    case NodeType::Expression:
      return visitExpression(node);
    case NodeType::Term:
      return visitTerm(node);
    case NodeType::Factor:
      return visitFactor(node);
    case NodeType::Literal:
      return visitLiteral(node);
    case NodeType::Operator:
      return visitOperator(node);
    case NodeType::UnaryMinus:
      return visitUnaryMinus(node);
    case NodeType::StringLiteral:
      return 0;
    case NodeType::PrintStatement:
      return visitPrintStatement(node);
    default:
      throw std::runtime_error("Unknown node type.");
  }
}

void Interpreter::executeStatement(std::shared_ptr<ASTNode> node) {
  std::shared_ptr<ASTNode> childNode = node->children[0];
  switch (node->type) {
    case NodeType::VarDeclaration:
      executeVarDeclaration(node);
      break;
    case NodeType::Assignment:
      executeAssignment(node);
      break;
    case NodeType::PrintStatement:
      if (childNode->type != NodeType::StringLiteral) {
        std::cout << "> " << visitPrintStatement(node) << std::endl;
      } else {
        visitPrintStatement(node);
      }
      break;

    case NodeType::IfStatement:
      executeIfStatement(node);
      break;

    case NodeType::WhileStatement:
      executeWhileStatement(node);
      break;

    default:
      throw std::runtime_error("Unknown statement type. 2");
  }
}

void Interpreter::executeIfStatement(std::shared_ptr<ASTNode> node) {
  // First child is the condition, second child is the body
  if (evaluateCondition(node->children[0])) {
    executeStatementList(node->children[1]);
  } else {
    // Handle elif and else parts if they exist
    for (size_t i = 2; i < node->children.size(); ++i) {
      auto child = node->children[i];
      if (child->type == NodeType::ElifStatement &&
          evaluateCondition(child->children[0])) {
        executeStatementList(child->children[1]);
        break;
      } else if (child->type == NodeType::ElseStatement) {
        executeStatementList(child->children[0]);
        break;
      }
    }
  }
}

void Interpreter::executeWhileStatement(std::shared_ptr<ASTNode> node) {
  while (evaluateCondition(node->children[0])) {
    executeStatementList(node->children[1]);
  }
}

bool Interpreter::evaluateCondition(std::shared_ptr<ASTNode> node) {
  int left = visit(node->children[0]);
  int right = visit(node->children[2]);
  std::string comparator = node->children[1]->value;

  if (comparator == "<")
    return left < right;
  else if (comparator == ">")
    return left > right;
  else if (comparator == "?") { /* Custom comparator logic */
    return left == right;
  } else if (comparator == "!") { /* Custom comparator logic */
    return left != right;
  } else
    throw std::runtime_error("Invalid comparator in comparison.");

  return false;  // Default case
}

void Interpreter::executeStatementList(std::shared_ptr<ASTNode> nodeList) {
  for (const auto& child : nodeList->children) {
    executeStatement(child);
  }
}

int Interpreter::visitIdentifier(std::shared_ptr<ASTNode> node) {
  std::string varName = node->value;
  if (symbolTable.find(varName) == symbolTable.end()) {
    throw std::runtime_error("Variable not declared: " + varName);
  }
  if (symbolTable[varName] == std::nullopt) {
    throw std::runtime_error("Variable used before assignment: " + varName);
  }
  return symbolTable[varName].value();
}

void Interpreter::executeVarDeclaration(std::shared_ptr<ASTNode> node) {
  std::string varName = node->children[1]->value;  // Identifier
  symbolTable[varName] = std::nullopt;

  if (node->children.size() > 2) {  // Check for expression
    int varValue = visit(node->children[2]);
    symbolTable[varName] = varValue;  // Expression
  }
}

void Interpreter::executeAssignment(std::shared_ptr<ASTNode> node) {
  std::string varName = node->children[0]->value;  // Identifier

  if (symbolTable.find(varName) == symbolTable.end()) {
    throw std::runtime_error("Variable not declared: " + varName);
  }

  int varValue = visit(node->children[1]);  // Expression
  symbolTable[varName] = varValue;
}

int Interpreter::visitUnaryMinus(std::shared_ptr<ASTNode> node) {
  return -visit(node->children[0]);
}

int Interpreter::visitOperator(std::shared_ptr<ASTNode> node) {
  return 0;
}

int Interpreter::visitLiteral(std::shared_ptr<ASTNode> node) {
  return std::stoi(node->value);
}

int Interpreter::visitExpression(std::shared_ptr<ASTNode> node) {
  int result = visit(node->children[0]);
  for (size_t i = 1; i < node->children.size(); i += 2) {
    auto op = node->children[i]->value;
    auto right = visit(node->children[i + 1]);
    if (op == "+") {
      result += right;
    } else if (op == "-") {
      result -= right;
    }
  }
  return result;
}

int Interpreter::visitTerm(std::shared_ptr<ASTNode> node) {
  int result = visit(node->children[0]);
  for (size_t i = 1; i < node->children.size(); i += 2) {
    auto op = node->children[i]->value;
    auto right = visit(node->children[i + 1]);
    if (op == "*") {
      result *= right;
    } else if (op == "/") {
      result /= right;
    }
  }
  return result;
}

int Interpreter::visitFactor(std::shared_ptr<ASTNode> node) {
  switch (node->type) {
    case NodeType::Literal:
      return std::stoi(node->value);
    case NodeType::Identifier:
      return visitIdentifier(node);
    case NodeType::Expression:
      return visitExpression(node);
    default:
      throw std::runtime_error("Invalid factor.");
  }
}

int Interpreter::visitPrintStatement(std::shared_ptr<ASTNode> node) {
  std::shared_ptr<ASTNode> childNode = node->children[0];
  switch (childNode->type) {
    case NodeType::Expression:
      return visitExpression(childNode);
    case NodeType::Identifier:
      return visitIdentifier(childNode);
    case NodeType::StringLiteral:
      std::cout << "> " << childNode->value << std::endl;
      return 0;
    default:
      throw std::runtime_error("Invalid node type in print statement.");
  }
}