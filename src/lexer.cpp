#include "lexer.h"
#include <iostream>

Lexer::Lexer(std::string& inputText)
    : inputText(inputText),
      position(Position(-1, 0, -1, inputText)),
      currentChar(),
      currentTokenIndex(0),
      nextTokenIndex(0) {
  advance();
}

const std::unordered_map<char, TokenType> operators = {
    {'+', TOKEN_OPERATOR},   {'-', TOKEN_OPERATOR},    {'*', TOKEN_OPERATOR},
    {'/', TOKEN_OPERATOR},   {'(', TOKEN_PARENTHESIS}, {')', TOKEN_PARENTHESIS},
    {'=', TOKEN_OPERATOR},   {'<', TOKEN_COMPARATOR},  {'>', TOKEN_COMPARATOR},
    {'?', TOKEN_COMPARATOR}, {'!', TOKEN_COMPARATOR},  {':', TOKEN_COLON}};

const std::unordered_map<std::string, TokenType> keywords = {
    {"if", TOKEN_KEYWORD},    {"elif", TOKEN_KEYWORD},  {"else", TOKEN_KEYWORD},
    {"while", TOKEN_KEYWORD}, {"var", TOKEN_KEYWORD},   {"int", TOKEN_KEYWORD},
    {"float", TOKEN_KEYWORD}, {"print", TOKEN_KEYWORD}, {"run", TOKEN_KEYWORD}};

void Lexer::advance() {
  position.advance(currentChar);
  currentChar = (position.getIndex() < static_cast<int>(inputText.size()))
                    ? inputText[position.getIndex()]
                    : '\0';
  nextChar = (position.getIndex() + 1 < static_cast<int>(inputText.size()))
                 ? inputText[position.getIndex() + 1]
                 : '\0';
}

void Lexer::skipWhitespace() {
  while (std::isspace(currentChar)) {
    advance();
  }
}

void Lexer::tokenize() {
  tokens.clear();
  Position posStart = position;
  bool lastTokenWasNewline = false;

  while (currentChar != '\0') {
    if (std::isspace(currentChar)) {
      if (currentChar == ' ' && nextChar == ' ') {
        tokens.push_back({TOKEN_INDENT, "  ", posStart, position});
        advance();
      } else if (currentChar == '\n' && !lastTokenWasNewline) {
        tokens.push_back({TOKEN_NEWLINE, "\\n", posStart, position});
        lastTokenWasNewline = true;
      } else if (currentChar != '\n') {
        lastTokenWasNewline = false;
      }
      advance();
    } else if (currentChar == '\"') {
      tokens.push_back(scanString(posStart));
      lastTokenWasNewline = false;
    } else if (std::isdigit(currentChar)) {
      tokens.push_back(scanNumber(posStart));
      lastTokenWasNewline = false;
    } else if (std::isalpha(currentChar) || currentChar == '_') {
      tokens.push_back(scanIdentifier(posStart));
      lastTokenWasNewline = false;
    } else if (operators.find(currentChar) != operators.end()) {
      tokens.push_back(scanOperator(posStart));
      lastTokenWasNewline = false;
    } else {
      throw IllegalCharError(posStart, position, std::string(1, currentChar));
    }
    posStart = position;
  }
}

Token Lexer::getNextToken() {
  if (currentTokenIndex < tokens.size()) {
    return tokens[currentTokenIndex++];
  }
  return {TOKEN_EOF, "", position, position};
}

Token Lexer::peekNextToken(size_t offset) {
  if (nextTokenIndex + offset < tokens.size()) {
    return tokens[nextTokenIndex++ + offset];
  }
  return {TOKEN_EOF, "", position, position};
}

std::vector<Token> Lexer::getAllTokens() const {
  return tokens;
}

Token Lexer::scanString(Position posStart) {
  advance();  // Skip the opening quote
  std::string value;
  while (currentChar != '\"' && currentChar != '\0') {
    value += currentChar;
    advance();
  }
  if (currentChar != '\"') {
    throw IllegalCharError(posStart, position, "Unterminated string literal.");
  }
  advance();  // Skip the closing quote
  return {TOKEN_STRING, value, posStart, position};
}

Token Lexer::scanNumber(Position posStart) {
  std::string number;
  bool dotFound = false;
  while (std::isdigit(currentChar) || currentChar == '.' ||
         std::isalpha(currentChar)) {
    if (std::isalpha(currentChar)) {
      throw IllegalCharError(
          posStart, position,
          "Invalid character in number: " + std::string(1, currentChar));
    }
    if (currentChar == '.') {
      if (dotFound) {
        throw IllegalCharError(posStart, position,
                               "Invalid number with multiple dots.");
      }
      dotFound = true;
      number += currentChar;
      advance();
      if (!std::isdigit(currentChar)) {
        throw IllegalCharError(posStart, position,
                               "Invalid number, a digit must follow a dot.");
      }
    }
    number += currentChar;
    advance();
  }

  return {(number.find('.') != std::string::npos) ? TOKEN_FLOAT : TOKEN_INTEGER,
          number, posStart, position};
}

Token Lexer::scanIdentifier(Position posStart) {
  std::string identifier;
  while (std::isalnum(currentChar) || currentChar == '_') {
    identifier += currentChar;
    advance();
  }
  auto keyword_it = keywords.find(identifier);
  if (keyword_it != keywords.end()) {
    return {keyword_it->second, identifier, posStart, position};
  } else {
    return {TOKEN_IDENTIFIER, identifier, posStart, position};
  }
}

Token Lexer::scanOperator(Position posStart) {
  auto operator_it = operators.find(currentChar);
  if (operator_it != operators.end()) {
    char foundChar = currentChar;
    advance();
    return {operator_it->second, std::string(1, foundChar), posStart, position};
  }
  return {TOKEN_INVALID, "", posStart, position};
}

const std::string& Lexer::getInputText() const {
  return inputText;
}

const Position& Lexer::getPosition() const {
  return position;
}

char Lexer::getCurrentChar() const {
  return currentChar;
}