//Dan Fu, December 2012
//This file contains the header for the Symbol Table class

//The definitions are found in table.cpp

#ifndef _table
#define _table

#include <string.h>
#include <iostream>
#include "general.h"

//Kinds for the Symbol Table
const int FUNC_ID = 10;
const int VAR_ID = 20;
const int REF_PARAM = 30;
const int VALUE_PARAM = 40;

//Types for the Symbol Table
const int FUNC_ID_TYPE = 100;
const int INT_TYPE = 200;
const int BOOLEAN_TYPE = 300;

using namespace std;

class tableNodeClass {
public:
  string lexeme;
  int kind;
  int type;
  int offset;
  int nestingLevel;
  tableNodeClass *params;
  tableNodeClass *next;
};

class scopeLevelClass {
public:
  string name;
  int nestingLevel;
  int nextOffset;
  tableNodeClass *item;
  scopeLevelClass *next;
};

class symbolTableClass {
public:
  symbolTableClass();
  void tableAddLevel(string levelName, tableNodeClass* params);
  void tableDeleteLevel();
  bool tableAddEntry(string lexeme, int kind, int type);
  bool tableAddParam(string lexeme, int kind, int type);
  tableNodeClass* tableLookup(string lex);
  void print();
  void tableDeleteAllTemps();
  void tableDeleteSpecialTemps();
private:
  scopeLevelClass *top;
  string typeToName(int type);
};

#endif