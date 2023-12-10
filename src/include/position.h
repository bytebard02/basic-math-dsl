// position.h
#ifndef POSITION_H
#define POSITION_H

#include <string>

class Position {
 public:
  Position(int index = 0,
           int line = 0,
           int column = 0,
           std::string inputText = "");
  Position advance(char currentChar);
  Position copy();

  int getIndex() const;
  int getLine() const;
  int getCol() const;
  const std::string& getInputText() const;

 private:
  int index;
  int line;
  int column;
  std::string inputText;
};

#endif