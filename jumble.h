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


class BadJumbleException:public std::exception {
  public:
  std::string errout;
  const char * what() const throw();
  BadJumbleException(int i);
};

class randomGen {
private:
  // Random number generator. Fully random, but entropy pool is limited so only being used for seeding.
  std::random_device rd;
  // 19937 state-size Mersenne Twister Engine. Not quite random, but more random than an LCG.
  std::mt19937 gen;

public:
  randomGen();
  int generate(int low, int high);
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
