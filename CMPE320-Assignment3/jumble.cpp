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
char JumblePuzzle::getDirection() {
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

JumblePuzzle::~JumblePuzzle() {
  // N is equivalent to rowlen from the generateJumble function.
  int N = ((int) word.length() * diff + 1)*3;
  for(int i = 0; i < N; i++) {
    free(puzzle[i]);
  }
  free(puzzle);
}

char **JumblePuzzle::generateJumble() {
  randomGen rn = randomGen();
  // row length (number of columns)
  int lenfac = (int) word.length() * diff + 1;
  int rowlen = lenfac * 3;
  
  // column length (number of rows)
  int collen = lenfac;
  
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
  
  // Set row and column labels
  for (int x = 0; x < collen; x++) {
    if(x < 10) {
      p[x+1][0] = ' ';
      p[x+1][1] = x + 48;
    }
    else {
      p[x+1][0] = (x - x%10)/10 + 48;
      p[x+1][1] = x%10 + 48;
    }
  }
  for (int x = 2; x < rowlen; x+=3) {
    if(x/3 < 10) {
      p[0][x+1] = ' ';
      p[0][x+2] = ' ';
      p[0][x+3] = x/3 + 48;
    }
    else {
      p[0][x+1] = ' ';
      p[0][x+2] = (x/3 - (x/3)%10)/10 + 48;
      p[0][x+3] = (x/3)%10 + 48;
    }
  }
  
  for (int x = 1; x < collen; x++) {
    for (int y = 2; y < rowlen; y+=3) {
      p[x][y+1] = ' ';
      p[x][y+2] = ' ';
      p[x][y+3] = rn.generate('a', 'z');
    }
  }
  
  // Randomly generate a direction either north, south, east, or west.
  int temp = rn.generate(0, 3);
  dir = (!temp) ? 'n' : (temp == 1) ? 's' : (temp == 2) ? 'w' : 'e';
  if (temp == 0) {
    dir = 'n';
    col = rn.generate(1, lenfac);
    // If we're going north, generate only values where the whole word will fit
    row = rn.generate(lenfac - (int) word.length() - 1, lenfac);
    int y = row;
    for (int x = 0; x < word.length(); x++) {
      p[y][col*3 + 2] = word[x];
      y--;
    }
  }
  else if (temp == 1) {
    dir = 's';
    col = rn.generate(1, lenfac);
    // If we're going north, generate only values where the whole word will fit
    row = rn.generate(1, lenfac - (int) word.length() - 1);
    int y = row*3 + 2;
    for (int x = 0; x < word.length(); x++) {
      p[y][col*3 + 2] = word[x];
      y++;
    }
  }
  else if (temp == 2) {
    dir = 'w';
    col = rn.generate(1, lenfac - (int) word.length() - 1);
    // If we're going north, generate only values where the whole word will fit
    row = rn.generate(1, lenfac);
    int y = col*3 + 2;
    for (int x = 0; x < word.length(); x++) {
      p[row][y] = word[x];
      y+=3;
    }
  }
  else {
    dir = 'e';
    col = rn.generate(lenfac - (int) word.length() - 1, lenfac);
    // If we're going north, generate only values where the whole word will fit
    row = rn.generate(1, lenfac);
    int y = col*3 + 2;
    for (int x = 0; x < word.length(); x++) {
      p[row][y] = word[x];
      y-=3;
    }
  }
  
  for (int x = 0; x < collen; x++) {
    for (int y = 0; y < rowlen; y++) {
      printf("%c", p[x][y]);
    }
    printf("\n");
  }
  // Accounting for overhead introduced in computation.
  row--;
  col--;
  return p;
}
