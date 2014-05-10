//Dan Fu, October 2012
//This file contains the header for the parser class.

//The definitions are found in parser.cpp

#ifndef _parser
#define _parser

#include "pstack.h"

//Entries in the precedence table
const int LT=-1;
const int GT=1;
const int EQ=0;
const int ER=-2;

class parseClass {
public:
  parseClass();
  void parse_expr();
  void parseProgram();
  void closeSourceProgram();
private:
  scannerClass scanner;
  tokenClass token;
  int precedence(tokenClass token1, tokenClass token2);
  string validRHS(tokenClass symbolsPopped [3], int arraySize);
  fstream outfile;
  tableNodeClass * lastReduced;
  int highestNestingLevel;
  int currentNestingLevel;
  void statement();
  void followIf(string elseLabel, string afterIfLabel);
  void followId(tableNodeClass *ident);
  void followCout();
  void ffCout();
  void fffCout();
  void followCin();
  void followBegin();
  void statementTail();
  int followExpression(tableNodeClass *params);
  void errorAndQuit(string s1, string s2);
  void program();
  int type();
  void block(string name, tableNodeClass* params);
  void progBody();
  void varDecs(int numVars);
  int identList(int identType);
  int identTailIdentList(int identType);
  void funcDecs();
  void funcDecTail();
  void paramList();
  void typeTail(int paramType);
  void identTailParamList();
  void duplicateIdentError(string lexeme);
  void undecVarError(string lexeme);
  void numParamError();
  string getNextTemp();
  void printInstruction(string instruction, tableNodeClass * ptr1,
			string str1, tableNodeClass * ptr2, string str2);
  string getNextSpecialTemp();
};

#endif