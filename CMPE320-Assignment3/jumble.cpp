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
/*
 BadJumbleException constructor.
 Case 0: word not valid length.
 Case 1: word contains invalid characters.
 Case 2: difficulty is invalid.
 Case 3: malloc failed.
 Default: generic error.
 */
BadJumbleException::BadJumbleException(int i) {
  switch(i) {
    case 0:
      errout = "ERROR (code 0): Input word not in length range 3-10.";
      break;
    case 1:
      errout = "ERROR (code 1): Input word contains non-alphabetical characters.";
      break;
    case 2:
      errout = "ERROR (code 2): Input difficulty is not \"easy\", \"medium\", or \"hard\".";
      break;
    case 3:
      errout = "ERROR (code 3): Memory allocation failed.";
      break;
    default:
      errout = "UNKNOWN ERROR: Unable to generate Jumble Puzzle.";
  }
}

const char * BadJumbleException::what() const throw() {
  return errout.c_str();
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
  // I just converted the input to lower case to improve usability
  if (inputword.length() < 3 || inputword.length() > 10) {
    throw BadJumbleException(0);
  }
  for (int x = 0; x < inputword.length(); x++) {
    // Throw an exception if any of the characters are not alphabetical.
    if(inputword[x] < 'A' || (inputword[x] > 'Z' && inputword[x] < 'a') || inputword[x] > 'z') {
      throw BadJumbleException(1);
    }
    if ( inputword[x] < 'a') {
      inputword[x] += 'a' - 'A';
    }
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
    throw BadJumbleException(2);
  }
  
  puzzle = generateJumble();
}

JumblePuzzle::JumblePuzzle(JumblePuzzle &jumble) {
  // Assign all parameters from the given puzzle to the new puzzle
  // (aside from the actual puzzle since it's a pointer)
  word = jumble.word;
  diff = jumble.diff;
  dir = jumble.dir;
  puzzleLength = jumble.puzzleLength;
  col = jumble.col;
  row = jumble.row;
  
  
  // Allocate space for the copied puzzle.
  int lenfac = (int) word.length() * diff;
  
  puzzle = (char **) malloc(lenfac * sizeof(char *));
  
  if(puzzle == NULL) {
    throw BadJumbleException(3);
  }
  for(int i = 0; i < lenfac; i++) {
    puzzle[i] = (char *) malloc(lenfac * sizeof(int));
    
    if(puzzle[i] == NULL) {
      throw BadJumbleException(3);
    }
  }
  
  // Populate new puzzle with characters from old puzzle.
  for (int x = 0; x < lenfac; x++) {
    for (int y = 0; y < lenfac; y++) {
      puzzle[x][y] = jumble.puzzle[x][y];
    }
  }
}

JumblePuzzle::~JumblePuzzle() {
  /*
  N is equivalent to lenfac from the generateJumble function.
   
  I know the delete function was mentioned but I figured this was
  slightly more direct and equivalent in functionality
   */
  int N = (int) word.length() * diff;
  for(int i = 0; i < N; i++) {
    free(puzzle[i]);
  }
  free(puzzle);
}

char **JumblePuzzle::generateJumble() {
  // Random generator.
  randomGen rn = randomGen();
  
  // Puzzle length
  int lenfac = (int) word.length() * diff;
  
  // row length
  int rowlen = lenfac;
  
  // column length
  int collen = lenfac;
  
  // Allocate space to p, making room for spacing and column/row labels and terminating characters
  char **p;
  p = (char **) malloc(rowlen * sizeof(char *));
  
  if(p == NULL) {
    throw BadJumbleException(3);
  }
  for(int i = 0; i < rowlen; i++) {
    p[i] = (char *) malloc(collen * sizeof(int));
    
    if(p[i] == NULL) {
      throw BadJumbleException(3);
    }
  }
  
  // Generate a grid of random lowercase characters.
  for (int x = 0; x < lenfac; x++) {
    for (int y = 0; y < lenfac; y++) {
      p[x][y] = rn.generate('a', 'z');
    }
  }
  
  // Randomly generate a direction either north, south, east, or west.
  int temp = rn.generate(0, 3);
  dir = (!temp) ? 'n' : (temp == 1) ? 's' : (temp == 2) ? 'w' : 'e';
  // North
  if (temp == 0) {
    dir = 'n';
    col = rn.generate(0, lenfac - 1);
    // If we're going north, generate only values where the whole word will fit
    row = rn.generate(lenfac - (int) word.length() - 1, lenfac - 1);
    int y = row;
    for (int x = 0; x < word.length(); x++) {
      p[y][col] = word[x];
      y--;
    }
  }
  // South
  else if (temp == 1) {
    dir = 's';
    col = rn.generate(0, lenfac - 1);
    // If we're going north, generate only values where the whole word will fit
    row = rn.generate(0, lenfac - (int) word.length() - 1);
    int y = row;
    for (int x = 0; x < word.length(); x++) {
      p[y][col] = word[x];
      y++;
    }
  }
  // West
  else if (temp == 2) {
    dir = 'w';
    col = rn.generate(0, lenfac - (int) word.length() - 1);
    // If we're going north, generate only values where the whole word will fit
    row = rn.generate(0, lenfac - 1);
    int y = col;
    for (int x = 0; x < word.length(); x++) {
      p[row][y] = word[x];
      y++;
    }
  }
  // East
  else {
    dir = 'e';
    col = rn.generate(lenfac - (int) word.length() - 1, lenfac - 1);
    // If we're going north, generate only values where the whole word will fit
    row = rn.generate(0, lenfac - 1);
    int y = col;
    for (int x = 0; x < word.length(); x++) {
      p[row][y] = word[x];
      y--;
    }
  }

  puzzleLength = lenfac;
  return p;
}
