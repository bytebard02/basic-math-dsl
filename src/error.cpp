#include "error.h"

Error::Error(const Position& posStart,
             const Position& posEnd,
             const std::string& errorName,
             const std::string& details)
    : posStart(posStart),
      posEnd(posEnd),
      errorName(errorName),
      details(details) {}

std::string Error::asString() const {
  std::string result = "\n\n" + std::to_string(posStart.getLine()) + ":" +
                       std::to_string(posStart.getCol()) + "-" +
                       std::to_string(posEnd.getLine()) + ":" +
                       std::to_string(posEnd.getCol()) + " > " + errorName +
                       ": " + details + "\n\n";
  return result;
}

IllegalCharError::IllegalCharError(const Position& posStart,
                                   const Position& posEnd,
                                   const std::string& details)
    : Error(posStart, posEnd, "Illegal Character(s)", details) {}

InvalidSyntaxError::InvalidSyntaxError(const Position& posStart,
                                       const Position& posEnd,
                                       const std::string& details)
    : Error(posStart, posEnd, "Illegal Syntax", details) {}