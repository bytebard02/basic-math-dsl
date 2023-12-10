#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "error.h"
#include "position.h"
#include "token.h"

class Lexer {
 public:
  Lexer(std::string& inputText);

  const std::string& getFileName() const;
  const std::string& getInputText() const;
  const Position& getPosition() const;
  char getCurrentChar() const;
  Token getNextToken();
  Token peekNextToken(std::size_t offset = 1);

  std::vector<Token> getAllTokens() const;
  void tokenize();

 private:
  std::string inputText;
  Position position;
  char currentChar;
  char nextChar;

  std::vector<Token> tokens;
  std::size_t currentTokenIndex;
  std::size_t nextTokenIndex;

  void advance();

  void skipWhitespace();

  Token scanString(Position posStart);
  Token scanNumber(Position posStart);
  Token scanIdentifier(Position posStart);
  Token scanOperator(Position posStart);
};
#endif