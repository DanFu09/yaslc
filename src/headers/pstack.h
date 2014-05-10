//Dan Fu, edited October 2012
//This file contains the headers for the pstack class
//and the stackCell class to be used in the
//expression parser

//The definitions are found in pstack.cpp

#ifndef _pstack
#define _pstack

#include "scanner.h"
#include <iostream>
#include "string"

using namespace std;

class stackCell {
public:
  int type;
  int subType;
  stackCell *next;
  tableNodeClass * symTableVariable;
};

class pStackClass {
public:
  pStackClass();
  void push(tokenClass theToken);
  tokenClass pop();
  bool isThereATerminalOnTop();
  tokenClass returnTopMostTerminal();
  void print();
private:
  stackCell *top;
};

#endif