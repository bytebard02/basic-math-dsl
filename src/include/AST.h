#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>

enum class NodeType {
  Program,
  Expression,
  Term,
  Factor,
  Literal,
  UnaryMinus,
  Operator,
  PrintStatement,
  VarDeclaration,
  Assignment,
  DataType,
  Identifier,
  StringLiteral,
  WhileStatement,
  IfStatement,
  ElifStatement,
  ElseStatement,
  Comparison,
  Comparator,
  StatementList
};

struct VariableInfo {
  std::string name;
  std::string dataType;
};

class ASTNode {
 public:
  ASTNode(NodeType type);
  void addChild(std::shared_ptr<ASTNode> child) { children.push_back(child); }

  static std::map<NodeType, std::string> nodeNames;

  NodeType type;
  std::vector<std::shared_ptr<ASTNode>> children;
  std::string value;

  std::string asString(int depth) const;

  std::string nodeName;
};