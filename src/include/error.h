#ifndef ERROR_H
#define ERROR_H

#include <string>
#include "position.h"

class Error {
 public:
  Error(const Position& posStart,
        const Position& posEnd,
        const std::string& errorName,
        const std::string& details);

  std::string asString() const;

 private:
  Position posStart;
  Position posEnd;
  std::string errorName;
  std::string details;
};

class IllegalCharError : public Error {
 public:
  IllegalCharError(const Position& posStart,
                   const Position& posEnd,
                   const std::string& details);
};

class InvalidSyntaxError : public Error {
 public:
  InvalidSyntaxError(const Position& posStart,
                     const Position& posEnd,
                     const std::string& details);
};

#endif