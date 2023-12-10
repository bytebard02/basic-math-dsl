#ifndef TOKENS_H
#define TOKENS_H

#include <map>
#include <string>
#include <unordered_map>
#include "position.h"

enum TokenType {
  TOKEN_KEYWORD,
  TOKEN_OPERATOR,
  TOKEN_COMPARATOR,
  TOKEN_IDENTIFIER,
  TOKEN_INTEGER,
  TOKEN_FLOAT,
  TOKEN_PARENTHESIS,
  TOKEN_COLON,
  TOKEN_NEWLINE,
  TOKEN_INVALID,
  TOKEN_STRING,
  TOKEN_INDENT,
  TOKEN_EOF
};

struct Token {
 public:
  Token() : type(TOKEN_INVALID), value("") {}
  Token(TokenType type,
        const std::string& value = "",
        const Position& posStart = Position(0, 0, 0, ""),
        const Position& posEnd = Position(0, 0, 0, ""));

  std::string asString() const;
  TokenType getType() const;
  const std::string& getValue() const;
  const Position& getPosStart() const;
  const Position& getPosEnd() const;

 private:
  TokenType type;
  std::string value;
  std::string tokenName;
  Position posStart;
  Position posEnd;

  static std::map<TokenType, std::string> tokenNames;
};
#endif