//
//  main.cpp
//  CMPE320-Assignment3
//
//  Created by Ethan Silver on 2022-11-08.
//

#include <iostream>
#include "jumble.h"

using namespace std;

// Define random number generator class.
randomGen::randomGen() {
  gen.seed(rd());
}
// This function will be used to generate all random values.
int randomGen::generate(int low, int high) {
  return uniform_int_distribution<>(low, high)(gen);
}

const char * BadJumbleException::what() const throw() {
  return "Invalid input! Unable to generate Jumble Puzzle.\n";
}

char ** JumblePuzzle::getJumble() {
  return puzzle;
}
int JumblePuzzle::getSize() {
  return puzzleLength;
}
int JumblePuzzle::getColPos() {
  return col;
}
int JumblePuzzle::getRowPos() {
  return row;
}
int JumblePuzzle::getDirection() {
  return dir;
}
    
  // Constructors
JumblePuzzle::JumblePuzzle(string inputword, string difficulty) {
  // Instead of checking for capitalization and throwing an error if the input has capitals,
  // I just converted the input to lower case
  for (int x = 0; x < inputword.length(); x++) {
    if ( inputword[x] < 'a') {
      inputword[x] += 'a' - 'A';
    }
  }
  if (inputword.length() < 3 || inputword.length() > 10) {
    throw BadJumbleException();
  }
  word = inputword;
  for (int x = 0; x < difficulty.length(); x++) {
    if ( difficulty[x] < 'a') {
      difficulty[x] += 'a' - 'A';
    }
  }
  
  if (!strcmp(difficulty.c_str(), "easy")) {
    diff = 2;
  }
  else if (!strcmp(difficulty.c_str(), "medium")) {
    diff = 3;
  }
  else if (!strcmp(difficulty.c_str(), "hard")) {
    diff = 4;
  }
  else {
    throw BadJumbleException();
  }
  
  puzzle = generateJumble();
}

JumblePuzzle::JumblePuzzle(JumblePuzzle &jumble) {
  jumble.word = word;
  jumble.diff = diff;
  jumble.generateJumble();
}

char **JumblePuzzle::generateJumble() {
  randomGen rn = randomGen();
  // row length (number of columns)
  int rowlen = (int) word.length() * diff * 3 + 3;
  
  // column length (number of rows)
  int collen = (int) word.length() * diff + 1;
  
  // Allocate space to p, making room for spacing and column/row labels and terminating characters
  char **p;
  p = (char **) malloc(rowlen * sizeof(char *));
  
  if(p == NULL) {
    throw BadJumbleException();
  }
  for(int i = 0; i < rowlen; i++) {
    p[i] = (char *) malloc(collen * sizeof(int));
    
    if(p[i] == NULL) {
      throw BadJumbleException();
    }
  }
  // p[x][y] where x is row and y is column
  // Top left corner is blank.
  p[0][0] = ' ';
  p[0][1] = ' ';
    
  for (int x = 1; x < collen; x++) {
    if(x <= 10) {
      p[x][0] = ' ';
      p[x][1] = x + 47;
    }
    else {
      p[x][0] = x - x%10 + 47;
      p[x][1] = x%10 + 47;
    }
  }
  for (int x = 2; x < rowlen; x+=3) {
    p[0][x+1] = ' ';
    p[0][x+2] = ' ';
    p[0][x+3] = x/3 + 48;
  }
  for (int x = 1; x < collen; x++) {
    for (int y = 2; y < rowlen; y+=3) {
      p[x][y+1] = ' ';
      p[x][y+2] = ' ';
      p[x][y+3] = rn.generate('a', 'z');
    }
  }
  
  for (int x = 0; x < collen; x++) {
    for (int y = 0; y < rowlen; y++) {
      printf("%c", p[x][y]);
    }
    printf("\n");
  }
    
  col = rn.generate(0, collen);
  row = rn.generate(0, rowlen);
  

  
  return p;
}
