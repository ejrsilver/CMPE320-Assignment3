//
//  jumblepuzzle.h
//  CMPE320-Assignment3
//
//  Created by Ethan Silver on 2022-11-08.
//

#ifndef jumble_h
#define jumble_h

#include <random>
#include <string>

// BadJumbleException
class BadJumbleException : public std::exception {
private:
  // Error code.
  int err;
public:
  const char * what() const throw();
  
  // Constructor with error code input.
  BadJumbleException(int i) : err(i) {}
};

class JumblePuzzle {
private:
  char **puzzle;
  int puzzleLength;
  int diff;
  char dir;
  std::string word;
  int col;
  int row;
  int generate(int low, int high);
  void initGen();
  
  char ** generateJumble();
  
public:
  char ** getJumble();
  int getSize();
  int getColPos();
  int getRowPos();
  char getDirection();
      
  // Constructors
  JumblePuzzle(JumblePuzzle &jumble);
  JumblePuzzle(std::string wordin, std::string diff);
  ~JumblePuzzle();
  
};

#endif /* jumblepuzzle_h */
