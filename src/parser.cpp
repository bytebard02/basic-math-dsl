#include "parser.h"

Parser::Parser(Lexer& lexer) : lexer(lexer) {
  advance();
}

std::map<NodeType, std::string> ASTNode::nodeNames = {
    {NodeType::Program, "Program"},
    {NodeType::Expression, "Expression"},
    {NodeType::Term, "Term"},
    {NodeType::Factor, "Factor"},
    {NodeType::Literal, "Literal"},
    {NodeType::Operator, "Operator"},
    {NodeType::UnaryMinus, "UnaryMinus"},
    {NodeType::PrintStatement, "PrintStatement"},
    {NodeType::VarDeclaration, "VarDeclaration"},
    {NodeType::Assignment, "Assignment"},
    {NodeType::DataType, "DataType"},
    {NodeType::Identifier, "Identifier"},
    {NodeType::StringLiteral, "StringLiteral"},
    {NodeType::IfStatement, "IfStatement"},
    {NodeType::ElifStatement, "ElifStatement"},
    {NodeType::ElseStatement, "ElseStatement"},
    {NodeType::WhileStatement, "WhileStatement"},
    {NodeType::Comparison, "Comparison"},
    {NodeType::Comparator, "Comparator"},
    {NodeType::StatementList, "IndentedStatementList"}};

ASTNode::ASTNode(NodeType type) : type(type), nodeName(nodeNames[type]) {}

void Parser::advance() {
  currentToken = lexer.getNextToken();
  nextToken = lexer.peekNextToken();
}

void Parser::eat(TokenType type) {
  if (currentToken.getType() == type) {
    advance();
  } else {
    std::string errorMsg = "Expected token of type " +
                           std::to_string(static_cast<int>(type)) +
                           ", got token of type " + currentToken.asString() +
                           " with value '" + currentToken.getValue() + "'";
    throw std::runtime_error(errorMsg);
  }
}

std::shared_ptr<ASTNode> Parser::parse() {
  auto programNode = std::make_shared<ASTNode>(NodeType::Program);
  while (currentToken.getType() != TokenType::TOKEN_EOF &&
         currentToken.getValue() != "run") {
    programNode->addChild(parseStatement());
    eat(TokenType::TOKEN_NEWLINE);  // Assuming TOKEN_NEWLINE represents '\n'
  }
  eat(TokenType::TOKEN_KEYWORD);  // Consume 'run'
  eat(TokenType::TOKEN_NEWLINE);  // Consume the newline after 'run'
  root = programNode;
  return programNode;
}

std::shared_ptr<ASTNode> Parser::parseStatement(int indentLevel) {
  if (currentToken.getType() == TokenType::TOKEN_KEYWORD) {
    if (currentToken.getValue() == "var") {
      return parseVarDeclaration();
    } else if (currentToken.getValue() == "print") {
      return parsePrintStatement();
    } else if (currentToken.getValue() == "if") {
      return parseIfStatement(indentLevel);
    } else if (currentToken.getValue() == "while") {
      return parseWhileStatement(indentLevel);
    }
  } else if (currentToken.getType() == TokenType::TOKEN_IDENTIFIER) {
    return parseAssignment();
  }
  throw std::runtime_error("Unexpected statement type." +
                           currentToken.asString());
}

std::shared_ptr<ASTNode> Parser::parseVarDeclaration() {
  eat(TokenType::TOKEN_KEYWORD);  // Consume 'var'
  auto dataTypeNode = parseDataType();
  auto identifierNode = parseIdentifier();

  auto varDeclNode = std::make_shared<ASTNode>(NodeType::VarDeclaration);
  varDeclNode->addChild(dataTypeNode);
  varDeclNode->addChild(identifierNode);

  if (currentToken.getType() == TokenType::TOKEN_OPERATOR) {  // Check for '='
    eat(TokenType::TOKEN_OPERATOR);                           // Consume '='
    auto expressionNode = parseExpression();
    varDeclNode->addChild(expressionNode);
  }

  return varDeclNode;
}

std::shared_ptr<ASTNode> Parser::parseAssignment() {
  auto identifierNode = parseIdentifier();
  eat(TokenType::TOKEN_OPERATOR);  // Consume '='
  auto expressionNode = parseExpression();

  auto assignmentNode = std::make_shared<ASTNode>(NodeType::Assignment);
  assignmentNode->addChild(identifierNode);
  assignmentNode->addChild(expressionNode);
  return assignmentNode;
}

std::shared_ptr<ASTNode> Parser::parseDataType() {
  if (currentToken.getType() != TokenType::TOKEN_KEYWORD ||
      (currentToken.getValue() != "int" &&
       currentToken.getValue() != "float")) {
    throw std::runtime_error("Expected data type (int or float)");
  }
  auto dataTypeNode = std::make_shared<ASTNode>(NodeType::DataType);
  dataTypeNode->value = currentToken.getValue();
  advance();
  return dataTypeNode;
}

std::shared_ptr<ASTNode> Parser::parseIdentifier() {
  if (currentToken.getType() != TokenType::TOKEN_IDENTIFIER) {
    throw std::runtime_error("Expected identifier");
  }
  auto identifierNode = std::make_shared<ASTNode>(NodeType::Identifier);
  identifierNode->value = currentToken.getValue();
  advance();
  return identifierNode;
}

std::shared_ptr<ASTNode> Parser::parseExpression() {
  auto node = std::make_shared<ASTNode>(NodeType::Expression);
  node->addChild(parseTerm());

  while (currentToken.getType() == TokenType::TOKEN_OPERATOR &&
         (currentToken.getValue() == "+" || currentToken.getValue() == "-")) {
    auto opNode = std::make_shared<ASTNode>(NodeType::Operator);
    opNode->value = currentToken.getValue();
    node->addChild(opNode);
    advance();
    node->addChild(parseTerm());
  }

  return node;
}

std::shared_ptr<ASTNode> Parser::parseTerm() {
  auto node = std::make_shared<ASTNode>(NodeType::Term);
  node->addChild(parseFactor());

  while (currentToken.getType() == TokenType::TOKEN_OPERATOR &&
         (currentToken.getValue() == "*" || currentToken.getValue() == "/")) {
    auto opNode = std::make_shared<ASTNode>(NodeType::Operator);
    opNode->value = currentToken.getValue();
    node->addChild(opNode);
    advance();
    node->addChild(parseFactor());
  }

  return node;
}

std::shared_ptr<ASTNode> Parser::parseFactor() {
  std::shared_ptr<ASTNode> node;

  if (currentToken.getType() == TokenType::TOKEN_OPERATOR &&
      currentToken.getValue() == "-") {
    advance();
    node = std::make_shared<ASTNode>(NodeType::UnaryMinus);
    node->addChild(parseFactor());
  } else if (currentToken.getType() == TokenType::TOKEN_INTEGER ||
             currentToken.getType() == TokenType::TOKEN_FLOAT) {
    node = std::make_shared<ASTNode>(NodeType::Literal);
    node->value = currentToken.getValue();
    advance();
  } else if (currentToken.getType() == TokenType::TOKEN_IDENTIFIER) {
    return parseIdentifier();
  } else if (currentToken.getType() == TokenType::TOKEN_PARENTHESIS) {
    eat(TokenType::TOKEN_PARENTHESIS);  // Consume '('
    auto node = parseExpression();
    eat(TokenType::TOKEN_PARENTHESIS);  // Consume ')'
    return node;
  } else {
    throw std::runtime_error("Invalid factor.");
  }

  return node;
}

std::shared_ptr<ASTNode> Parser::parsePrintStatement() {
  if (currentToken.getType() == TokenType::TOKEN_KEYWORD &&
      currentToken.getValue() == "print") {
    eat(TokenType::TOKEN_KEYWORD);
  }

  std::shared_ptr<ASTNode> node =
      std::make_shared<ASTNode>(NodeType::PrintStatement);

  if (currentToken.getType() == TokenType::TOKEN_STRING) {
    auto stringLiteralNode = std::make_shared<ASTNode>(NodeType::StringLiteral);
    stringLiteralNode->value = currentToken.getValue();
    node->addChild(stringLiteralNode);
    eat(TokenType::TOKEN_STRING);
  } else {
    node->addChild(parseExpression());
  }

  return node;
}

std::shared_ptr<ASTNode> Parser::parseIfStatement(int indentLevel) {
  eat(TokenType::TOKEN_KEYWORD);      // Consume 'if'
  eat(TokenType::TOKEN_PARENTHESIS);  // Consume '('
  auto condition = parseComparison();
  eat(TokenType::TOKEN_PARENTHESIS);  // Consume ')'
  eat(TokenType::TOKEN_COLON);        // Consume ':'
  eat(TokenType::TOKEN_NEWLINE);      // Consume '\n'
  auto body = parseIndentedStatementList(0);

  auto ifNode = std::make_shared<ASTNode>(NodeType::IfStatement);
  ifNode->addChild(condition);
  ifNode->addChild(body);

  // Handle 'elif' and 'else' parts
  while (true) {
    if (currentToken.getType() == TokenType::TOKEN_KEYWORD &&
        currentToken.getValue() == "elif") {
      ifNode->addChild(parseElifStatement(indentLevel));
    } else if (currentToken.getType() == TokenType::TOKEN_KEYWORD &&
               currentToken.getValue() == "else") {
      ifNode->addChild(parseElseStatement(indentLevel));
      break;  // Only one 'else' is allowed, so break after parsing it
    } else {
      break;  // If it's not 'elif' or 'else', exit the loop
    }
  }

  return ifNode;
}

std::shared_ptr<ASTNode> Parser::parseElifStatement(int indentLevel) {
  eat(TokenType::TOKEN_KEYWORD);      // Consume 'elif'
  eat(TokenType::TOKEN_PARENTHESIS);  // Consume '('
  auto condition = parseComparison();
  eat(TokenType::TOKEN_PARENTHESIS);  // Consume ')'
  eat(TokenType::TOKEN_COLON);        // Consume ':'
  eat(TokenType::TOKEN_NEWLINE);      // Consume '\n'
  auto body = parseIndentedStatementList(indentLevel);

  auto elifNode = std::make_shared<ASTNode>(NodeType::ElifStatement);
  elifNode->addChild(condition);
  elifNode->addChild(body);

  return elifNode;
}

std::shared_ptr<ASTNode> Parser::parseElseStatement(int indentLevel) {
  eat(TokenType::TOKEN_KEYWORD);  // Consume 'else'
  eat(TokenType::TOKEN_COLON);    // Consume ':'
  eat(TokenType::TOKEN_NEWLINE);  // Consume '\n'
  auto body = parseIndentedStatementList(indentLevel);

  auto elseNode = std::make_shared<ASTNode>(NodeType::ElseStatement);
  elseNode->addChild(body);

  return elseNode;
}

std::shared_ptr<ASTNode> Parser::parseWhileStatement(int indentLevel) {
  eat(TokenType::TOKEN_KEYWORD);      // Consume 'while'
  eat(TokenType::TOKEN_PARENTHESIS);  // Consume '('
  auto condition = parseComparison();
  eat(TokenType::TOKEN_PARENTHESIS);  // Consume ')'
  eat(TokenType::TOKEN_COLON);        // Consume ':'
  eat(TokenType::TOKEN_NEWLINE);      // Consume '\n'
  auto body = parseIndentedStatementList(indentLevel);

  auto whileNode = std::make_shared<ASTNode>(NodeType::WhileStatement);
  whileNode->addChild(condition);
  whileNode->addChild(body);

  return whileNode;
}

std::shared_ptr<ASTNode> Parser::parseComparison() {
  auto left = parseExpression();
  auto comparator = parseComparator();
  auto right = parseExpression();

  auto comparisonNode = std::make_shared<ASTNode>(NodeType::Comparison);
  comparisonNode->addChild(left);
  comparisonNode->addChild(comparator);
  comparisonNode->addChild(right);

  return comparisonNode;
}

std::shared_ptr<ASTNode> Parser::parseComparator() {
  if (currentToken.getType() != TokenType::TOKEN_COMPARATOR &&
      (currentToken.getValue() != "<" && currentToken.getValue() != ">" &&
       currentToken.getValue() != "?" && currentToken.getValue() != "!")) {
    throw std::runtime_error("Expected comparator (<, >, ?, or !)");
  }

  auto comparatorNode = std::make_shared<ASTNode>(NodeType::Comparator);
  comparatorNode->value = currentToken.getValue();
  advance();  // Consume the comparator token

  return comparatorNode;
}

std::shared_ptr<ASTNode> Parser::parseIndentedStatementList(int indentLevel) {
  auto indentedBlockNode = std::make_shared<ASTNode>(NodeType::StatementList);

  int currentIndentLevel = indentLevel;
  indentLevel++;

  while (currentToken.getType() == TokenType::TOKEN_INDENT) {
    for (int i = 0; i < indentLevel; i++) {
      eat(TokenType::TOKEN_INDENT);
    }

    indentedBlockNode->addChild(parseStatement(indentLevel));

    if (nextToken.getType() != TokenType::TOKEN_INDENT) {
      break;
    }
    eat(TokenType::TOKEN_NEWLINE);
  }
  std::cout << "indentLevel: " << indentLevel << std::endl;
  return indentedBlockNode;
}

std::string ASTNode::asString(int depth) const {
  std::string indent(depth * 2, ' ');
  std::string result = indent + "Node type: " + nodeName + "\n";
  if (!value.empty()) {
    result += indent + "Value: " + value + "\n";
  }
  for (const auto& child : children) {
    result += child->asString(depth + 1);
  }
  return result;
}

std::shared_ptr<ASTNode> Parser::getAST() {
  return root;
}
