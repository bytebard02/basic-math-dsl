#include "token.h"
#include "position.h"

Token::Token(TokenType type,
             const std::string& value,
             const Position& posStart,
             const Position& posEnd)
    : type(type),
      value(value),
      tokenName(tokenNames[type]),
      posStart(posStart),
      posEnd(posEnd) {}

std::map<TokenType, std::string> Token::tokenNames = {
    {TOKEN_KEYWORD, "TOKEN_KEYWORD"},
    {TOKEN_PARENTHESIS, "TOKEN_PARENTHESIS"},
    {TOKEN_COLON, "TOKEN_COLON"},
    {TOKEN_IDENTIFIER, "TOKEN_IDENTIFIER"},
    {TOKEN_INTEGER, "TOKEN_INTEGER"},
    {TOKEN_OPERATOR, "TOKEN_OPERATOR"},
    {TOKEN_COMPARATOR, "TOKEN_COMPARATOR"},
    {TOKEN_INTEGER, "TOKEN_INTEGER"},
    {TOKEN_FLOAT, "TOKEN_FLOAT"},
    {TOKEN_NEWLINE, "TOKEN_NEWLINE"},
    {TOKEN_STRING, "TOKEN_STRING"},
    {TOKEN_INDENT, "TOKEN_INDENT"},
    {TOKEN_EOF, "TOKEN_EOF"}};

std::string Token::asString() const {
  return "Token(" + tokenName + ", '" + value + "' ln:col " +
         std::to_string(posStart.getLine()) + ":" +
         std::to_string(posStart.getCol()) + " " +
         std::to_string(posEnd.getLine()) + ":" +
         std::to_string(posEnd.getCol()) + ")";
}

TokenType Token::getType() const {
  return type;
}

const std::string& Token::getValue() const {
  return value;
}

const Position& Token::getPosStart() const {
  return posStart;
}

const Position& Token::getPosEnd() const {
  return posEnd;
}

Position::Position(int index, int line, int column, std::string inputText)
    : index(index), line(line), column(column), inputText(inputText) {}

Position Position::advance(char currentChar) {
  index++;
  column++;

  if (currentChar == '\n') {
    line++;
    column = 0;
  }

  return *this;
}

Position Position::copy() {
  return Position(index, line, column, inputText);
}

int Position::getIndex() const {
  return index;
}

int Position::getLine() const {
  return line;
}

int Position::getCol() const {
  return column;
}

const std::string& Position::getInputText() const {
  return inputText;
}
