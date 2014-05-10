//Dan Fu, September 2012
//This file contains the header for the scanner/lexer
//class as well as the token class.

//The definitions are found in scanner.cpp

#ifndef _scanner
#define _scanner

#include <string>
#include <iostream>
#include "filemngr.h"
#include "general.h"
#include "table.h"
#include "cctype"

//types
const int EOF_T=0;
const int ADDOP_T=1;
const int MULOP_T=2;
const int RELOP_T=3;
const int ASSIGNMENT_T=5;
const int LPAR_T=6;
const int RPAR_T=7;
const int SEMICOLON_T=8;
const int COMMA_T=9;
const int GL_T=10;
const int PERIOD_T=11;
const int AMPERSAND_T=12;
const int IDENTIFIER_T=13;
const int STRING_T=14;
const int INTEGER_T=15;
const int PROGRAM_T=16;
const int FUNCTION_T=17;
const int BEGIN_T=18;
const int END_T=19;
const int IF_T=20;
const int THEN_T=21;
const int ELSE_T=22;
const int WHILE_T=23;
const int DO_T=24;
const int COUT_T=25;
const int CIN_T=26;
const int ENDL_T=27;
const int INT_T=28;
const int BOOLEAN_T=29;
const int TRUE_T=30;
const int FALSE_T=31;
const int LL_T=32;
const int GG_T=33;

//Errors
const int ERROR_T=99;

//subtypes
//miscellaneous
const int NONE_ST=100;

//addops
const int OR_ST=200;
const int PLUS_ST=201;
const int MINUS_ST=202;

//mulops
const int AND_ST=300;
const int ASTERISK_ST=301;
const int DIV_ST=302;
const int MOD_ST=303;

//relops
const int LT_ST=400;
const int LEQ_ST=401;
const int EQUAL_ST=403;
const int GEQ_ST=404;
const int GT_ST=405;
const int NEQ_ST=407;

//For the Syntax Parser
const int E_T=1000;
const int DOLLAR_SIGN_T=1001;

using namespace std;

class tokenClass {
public:
  tokenClass();
  tokenClass(int type, int subtype, string lexeme);
  void display();
  string getType();
  string getSubtype();
  int getTypeNum();
  int getSubtypeNum();
  string getLexeme();
  tableNodeClass * symTableVariable;
private:
  int myType;
  int mySubtype;
  string myLexeme;
};

class scannerClass {
public:
  scannerClass();
  tokenClass getNextToken();
  void closeSourceProgram();
  void printCurrentLine();
  bool expressionDebuggingStatus;
  void tableAddLevel(string levelName, tableNodeClass* params);
  void tableDeleteLevel();
  bool tableAddEntry(string lexeme, int kind, int type);
  bool tableAddParam(string lexeme, int kind, int type);
  tableNodeClass* tableLookup(string lex);
  void tableDeleteAllTemps();
  void tableDeleteSpecialTemps();
private:
  fileManagerClass theFile;
  string theLexeme;
  symbolTableClass theTable;
  int getNextState(int curState, char input);
  int inputIndex(char input);
  int stateIndex(int curState);
};

#endif