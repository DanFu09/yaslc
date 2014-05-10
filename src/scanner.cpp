//Dan Fu, September 2012
//This file contains the implementations of
//the scanner/lexer class and the token class.

//The header is found in SCANNER.h

#include "headers/scanner.h"

tokenClass::tokenClass() {
  /**
   D *efault constructor for the tokenClass, simply initializes
   the instance fields.
   */
  myType=ERROR_T;
  mySubtype=NONE_ST;
  myLexeme="";
}

tokenClass::tokenClass(int type, int subtype, std::string lexeme) {
  /**
   C *onstructor for the tokenClass that takes in type, subtype,
   and lexeme.
   */
  myType = type;
  mySubtype = subtype;
  myLexeme = lexeme;
  symTableVariable = NULL;
}

string tokenClass::getType() {
  /**
   R *eturns a tokenClass's type in text format.
   */
  switch(myType) {
    case EOF_T:
      return "EOF_T";
    case ADDOP_T:
      return "ADDOP_T";
    case MULOP_T:
      return "MULOP_T";
    case RELOP_T:
      return "RELOP_T";
    case ASSIGNMENT_T:
      return "ASSIGNMENT_T";
    case LPAR_T:
      return "LPAR_T";
    case RPAR_T:
      return "RPAR_T";
    case SEMICOLON_T:
      return "SEMICOLON_T";
    case COMMA_T:
      return "COMMA_T";
    case GL_T:
      return "GL_T";
    case PERIOD_T:
      return "PERIOD_T";
    case AMPERSAND_T:
      return "AMPERSAND_T";
    case IDENTIFIER_T:
      return "IDENTIFIER_T";
    case STRING_T:
      return "STRING_T";
    case INTEGER_T:
      return "INTEGER_T";
    case PROGRAM_T:
      return "PROGRAM_T";
    case FUNCTION_T:
      return "FUNCTION_T";
    case BEGIN_T:
      return "BEGIN_T";
    case END_T:
      return "END_T";
    case IF_T:
      return "IF_T";
    case THEN_T:
      return "THEN_T";
    case ELSE_T:
      return "ELSE_T";
    case WHILE_T:
      return "WHILE_T";
    case DO_T:
      return "DO_T";
    case COUT_T:
      return "COUT_T";
    case CIN_T:
      return "CIN_T";
    case ENDL_T:
      return "ENDL_T";
    case INT_T:
      return "INT_T";
    case BOOLEAN_T:
      return "BOOLEAN_T";
    case TRUE_T:
      return "TRUE_T";
    case FALSE_T:
      return "FALSE_T";
    case ERROR_T:
      return "ERROR_T";
    case LL_T:
      return "LL_T";
    case GG_T:
      return "GG_T";
    default:
      return "UNKNOWN";
  }
  return "UKNOWN";
}

int tokenClass::getTypeNum() {
  /**
   R *eturns a tokenClass's type in number format.
   */
  return myType;
}
string tokenClass::getSubtype() {
  /**
   R *eturns a tokenClass's subtype in text format.
   */
  switch(mySubtype) {
    case NONE_ST:
      return "NONE_ST";
    case OR_ST:
      return "OR_ST";
    case PLUS_ST:
      return "PLUS_ST";
    case MINUS_ST:
      return "MINUS_ST";
    case AND_ST:
      return "AND_ST";
    case ASTERISK_ST:
      return "ASTERISK_ST";
    case DIV_ST:
      return "DIV_ST";
    case MOD_ST:
      return "MOD_ST";
    case LT_ST:
      return "LT_ST";
    case LEQ_ST:
      return "LEQ_ST";
    case EQUAL_ST:
      return "EQUAL_ST";
    case GT_ST:
      return "GT_ST";
    case GEQ_ST:
      return "GEQ_ST";
    case NEQ_ST:
      return "NEQ_ST";
    default:
      return "NONE_ST";
  }
  return "NONE_ST";
}

int tokenClass::getSubtypeNum() {
  /**
   R *eturns a tokenClass's subtype in number format.
   */
  return mySubtype;
}
string tokenClass::getLexeme() {
  /**
   R *eturns the tokenClass's lexeme.
   */
  return myLexeme;
}

void tokenClass::display() {
  /**
   P *rints the tokenClass in the following format:
   LEXEME		TYPE		SUBTYPE
   */
  cout << getLexeme() << "\t\t" << getType() << "\t\t" << getSubtype() << endl;
}

void scannerClass::closeSourceProgram() {
  /**
   C *loses the source program by calling the file manager's
   closeSourceProgram() method.
   */
  theFile.closeSourceProgram();
}

int scannerClass::inputIndex(char input) {
  /**
   T *akes in a character input and returns the index
   of the two-dimensional FSA array that the input
   corresponds to.
   */
  int charIndex = 0;  //Index in the FSA array the input corresponds to
  //Assign charIndex correctly
  //EOF=0,+=1,-=2,==3,<=4,>=5,*=6,&=7,(=9,)=10,,=11,.=12,;=13,{=14,}=15,
  //$=16,cr=17,ws=18,l=19,d=20,_=21,'=22,/=23,other=24
  switch(input) {
    case (EOF):
      return 0;
    case '+':
      return 1;
    case '-':
      return 2;
    case '=':
      return 3;
    case '<':
      return 4;
    case '>':
      return 5;
    case '*':
      return 6;
    case '&':
      return 7;
    case '(':
      return 9;
    case ')':
      return 10;
    case ',':
      return 11;
    case '.':
      return 12;
    case ';':
      return 13;
    case '{':
      return 14;
    case '}':
      return 15;
    case '$':
      return 16;
    case '\n':
      return 17;
    case '\r':
      return 17;
    case '_':
      return 21;
    case '\'':
      return 22;
    case '/':
      return 23;
    default:
      charIndex=24;
  }
  if (input == ' ' || input == '\t') { charIndex=18; }
  if (islower(input) || isupper(input)) { charIndex=19; }
  if (isdigit(input)) { charIndex=20; }
  
  return charIndex;
}

int scannerClass::stateIndex(int curState) {
  /**
   T *akes in the state number (as in the number given
   on the actual FSA diagram) and returns the index in the FSA
   array that the number corresponds to.
   */
  //0=0,1=1,2=2,3=3,4=4,5=5,6=6,7=7,8=8,13=9,14=10,17=11,9=12,11=13,12=14
  switch (curState) {
    case 0:
      return 0;
    case 1:
      return 1;
    case 2:
      return 2;
    case 3:
      return 3;
    case 4:
      return 4;
    case 5:
      return 5;
    case 6:
      return 6;
    case 7:
      return 7;
    case 8:
      return 8;
    case 13:
      return 9;
    case 14:
      return 10;
    case 17:
      return 11;
    case 9:
      return 12;
    case 11:
      return 13;
    case 12:
      return 14;
    default:
      return 0;
  }
}

int scannerClass::getNextState(int curState, char input) {
  /**
   T *akes in a state number and an input and gives the next
   state number. Finds the next state number by using
   fsaArray[stateIndex][inputIndex].
   @param curState the state number in the format given on the
   FSA diagram.
   @param input the character input given by the file manager's
   getNextChar() method.
   @return the next state number in the format given on the FSA
   diagram.
   */
  int charIndex=inputIndex(input);
  int curStateIndex=stateIndex(curState);
  int nextState=0;
  
  //Array index: fsaArray[i][j] means state i, input j
  static int fsaArray [15][25] = {
    { 103, 100, 101, 3, 4, 5, 102, 121, 0, 114, 115, 117, 120, 116, 6, 118, 118, 0, 0, 11, 9, 118, 12, 1, 118 }, //0=0
    { 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 2, 104 }, //1=1
    { 103, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2 }, //2=2
    { 106, 106, 106, 105, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106 }, //3=3
    { 110, 110, 110, 107, 108, 109, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110 }, //4=4
    { 113, 113, 113, 111, 119, 112, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113 }, //5=5
    { 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 7, 17, 17, 17, 17, 17, 17, 17, 17 }, //6=6
    { 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 8, 17, 17, 17, 17, 17 }, //7=7
    { 17, 13, 13, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17 }, //8=8
    { 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 124, 17, 17, 17, 17, 17, 17, 17, 17, 17 }, //13=9
    { 123, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 0, 14, 14, 14, 14, 14, 14, 14, 14, 14 }, //14=10
    { 123, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 0, 14, 14, 14, 14, 14, 14, 14, 14, 14 }, //17=11
    { 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 9, 127, 127, 127, 127 }, //9=12
    { 129, 129, 129, 129, 129, 129, 129, 129, 129, 129, 129, 129, 129, 129, 129, 129, 129, 129, 129, 11, 11, 11, 129, 129, 129 }, //11=13
    { 131, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 130, 12, 12, 12, 12, 132, 12, 12 } // 12=14
  };
  
  nextState=fsaArray[curStateIndex][charIndex];
  return nextState;
}

tokenClass scannerClass::getNextToken() {
  /**
   R *eturns the next token in the file. It does this by
   continuously calculating the next state using getNextState
   and performing the functions associated with each new state
   until a new token is declared, an error is met, or the file
   ends.
   This is the method that must be modified to add new compiler directives.
   */
  int state=0;
  char nextInput;
  theLexeme="";
  tokenClass theToken (ERROR_T, NONE_ST, theLexeme);
  do {
    nextInput=theFile.getNextChar();
    state=getNextState(state, nextInput);
    theLexeme += nextInput;
    switch (state) {
      case 0:
	theLexeme="";
	break;
      case 100: 
	theToken = tokenClass(ADDOP_T, PLUS_ST, theLexeme);
	return theToken;
      case 101:
	theToken = tokenClass(ADDOP_T, MINUS_ST, theLexeme);
	return theToken;
      case 102:
	theToken = tokenClass(MULOP_T, ASTERISK_ST, theLexeme);
	return theToken;
      case 103:
	theToken = tokenClass(EOF_T, NONE_ST, "EOF");
	return theToken;
      case 104:
	theToken = tokenClass(ERROR_T, NONE_ST, "ERROR");
	cerr << "Error:" << endl << "\t";
	theFile.printCurrentLine();
	cerr << "\tUnexpected /" << endl;
	return theToken;
      case 2: //In a one-line comment
	theLexeme = "";
	continue;
      case 105:
	theToken=tokenClass(RELOP_T, EQUAL_ST, theLexeme);
	return theToken;
      case 106:
	theFile.pushBack();
	theLexeme.erase(theLexeme.length()-1,1);
	theToken=tokenClass(ASSIGNMENT_T, NONE_ST, theLexeme);
	return theToken;
      case 107:
	theToken=tokenClass(RELOP_T, LEQ_ST, theLexeme);
	return theToken;
      case 108:
	theToken=tokenClass(LL_T, NONE_ST, theLexeme);
	return theToken;
      case 109:
	theToken=tokenClass(RELOP_T, NEQ_ST, theLexeme);
	return theToken;
      case 110:
	theFile.pushBack();
	theLexeme.erase(theLexeme.length()-1,1);
	theToken=tokenClass(RELOP_T, LT_ST, theLexeme);
	return theToken;
      case 111:
	theToken=tokenClass(RELOP_T, GEQ_ST, theLexeme);
	return theToken;
      case 112:
	theToken=tokenClass(GG_T, NONE_ST, theLexeme);
	return theToken;
      case 113:
	theFile.pushBack();
	theLexeme.erase(theLexeme.length()-1,1);
	theToken=tokenClass(RELOP_T, GT_ST, theLexeme);
	return theToken;
      case 114:
	theToken=tokenClass(LPAR_T, NONE_ST, theLexeme);
	return theToken;
      case 115:
	theToken=tokenClass(RPAR_T, NONE_ST, theLexeme);
	return theToken;
      case 116:
	theToken=tokenClass(SEMICOLON_T, NONE_ST, theLexeme);
	return theToken;
      case 117:
	theToken=tokenClass(COMMA_T, NONE_ST, theLexeme);
	return theToken;
      case 118:
	theToken=tokenClass(ERROR_T, NONE_ST, theLexeme);
	cerr << "Error:" << endl << "\t";
	theFile.printCurrentLine();
	cerr << "\tUnexpected " << theLexeme << endl;
	return theToken;
      case 119:
	theToken=tokenClass(GL_T, NONE_ST, theLexeme);
	return theToken;
      case 120:
	theToken=tokenClass(PERIOD_T, NONE_ST, theLexeme);
	return theToken;
      case 121:
	theToken=tokenClass(AMPERSAND_T, NONE_ST, theLexeme);
	return theToken;
      case 123:
	theToken=tokenClass(ERROR_T, NONE_ST, theLexeme);
	cerr << "Error:" << endl << "\t";
	theFile.printCurrentLine();
	cerr << "\tUnclosed {" << endl;
	  return theToken;
      case 124:
	if (theLexeme[2] == 'p') {
	  theFile.setPrintStatus(theLexeme[3]=='+');
	}
	else if (theLexeme[2] == 'e') {
	  expressionDebuggingStatus=theLexeme[3]=='+';
	}
	else if (theLexeme[2] == 's') {
	  theTable.print();
	}
	else {
	  cerr << "Warning:\n\tCompiler directive " << theLexeme[2] << " unknown." << endl << "\t";
	  theFile.printCurrentLine();
	}
	state=0;
	theLexeme="";
	break;
      case 14:
	theLexeme="";
	break;
      case 17:
	theFile.pushBack();
	theLexeme="";
	break;
      case 127:
	theFile.pushBack();
	theLexeme.erase(theLexeme.length()-1,1);
	if (theLexeme.length() > 4) {
	  theToken=tokenClass(ERROR_T, NONE_ST, theLexeme);
	  cerr << "Error:" << endl << "\t";
	  theFile.printCurrentLine();
	  cerr << "\tInteger " << theLexeme.length() << " digits long." << endl;
	  return theToken;
	}
	theToken=tokenClass(INTEGER_T, NONE_ST, theLexeme);
	return theToken;
      case 129:
	theFile.pushBack();
	theLexeme.erase(theLexeme.length()-1,1);
	if (strcmp(theLexeme.c_str(),"program")==0) {
	  theToken=tokenClass(PROGRAM_T, NONE_ST, theLexeme);
	  return theToken;
	}
	if (strcmp(theLexeme.c_str(),"function")==0) {
	  theToken=tokenClass(FUNCTION_T, NONE_ST, theLexeme);
	  return theToken;
	}
	if (strcmp(theLexeme.c_str(),"begin")==0) {
	  theToken=tokenClass(BEGIN_T, NONE_ST, theLexeme);
	  return theToken;
	}
	if (strcmp(theLexeme.c_str(),"end")==0) {
	  theToken=tokenClass(END_T, NONE_ST, theLexeme);
	  return theToken;
	}
	if (strcmp(theLexeme.c_str(),"if")==0) {
	  theToken=tokenClass(IF_T, NONE_ST, theLexeme);
	  return theToken;
	}
	if (strcmp(theLexeme.c_str(),"then")==0) {
	  theToken=tokenClass(THEN_T, NONE_ST, theLexeme);
	  return theToken;
	}
	if (strcmp(theLexeme.c_str(),"else")==0) {
	  theToken=tokenClass(ELSE_T, NONE_ST, theLexeme);
	  return theToken;
	}
	if (strcmp(theLexeme.c_str(),"while")==0) {
	  theToken=tokenClass(WHILE_T, NONE_ST, theLexeme);
	  return theToken;
	}
	if (strcmp(theLexeme.c_str(),"do")==0) {
	  theToken=tokenClass(DO_T, NONE_ST, theLexeme);
	  return theToken;
	}
	if (strcmp(theLexeme.c_str(),"cout")==0) {
	  theToken=tokenClass(COUT_T, NONE_ST, theLexeme);
	  return theToken;
	}
	if (strcmp(theLexeme.c_str(),"cin")==0) {
	  theToken=tokenClass(CIN_T, NONE_ST, theLexeme);
	  return theToken;
	}
	if (strcmp(theLexeme.c_str(),"endl")==0) {
	  theToken=tokenClass(ENDL_T, NONE_ST, theLexeme);
	  return theToken;
	}
	if (strcmp(theLexeme.c_str(),"or")==0) {
	  theToken=tokenClass(ADDOP_T, OR_ST, theLexeme);
	  return theToken;
	}
	if (strcmp(theLexeme.c_str(),"and")==0) {
	  theToken=tokenClass(MULOP_T, AND_ST, theLexeme);
	  return theToken;
	}
	if (strcmp(theLexeme.c_str(),"div")==0) {
	  theToken=tokenClass(MULOP_T, DIV_ST, theLexeme);
	  return theToken;
	}
	if (strcmp(theLexeme.c_str(),"mod")==0) {
	  theToken=tokenClass(MULOP_T, MOD_ST, theLexeme);
	  return theToken;
	}
	if (strcmp(theLexeme.c_str(),"int")==0) {
	  theToken=tokenClass(INT_T, NONE_ST, theLexeme);
	  return theToken;
	}
	if (strcmp(theLexeme.c_str(),"boolean")==0) {
	  theToken=tokenClass(BOOLEAN_T, NONE_ST, theLexeme);
	  return theToken;
	}
	if (strcmp(theLexeme.c_str(),"true")==0) {
	  theToken=tokenClass(TRUE_T, NONE_ST, theLexeme);
	  return theToken;
	}
	if (strcmp(theLexeme.c_str(),"false")==0) {
	  theToken=tokenClass(FALSE_T, NONE_ST, theLexeme);
	  return theToken;
	}
	if (theLexeme.length() > 12) {
	  theToken=tokenClass(ERROR_T, NONE_ST, theLexeme);
	  cerr << "Error:" << endl;
	  theFile.printCurrentLine();
	  cerr << "\tIdentifier " << theLexeme.length() << " characters long." << endl;
	  return theToken;
	}
	theToken=tokenClass(IDENTIFIER_T, NONE_ST, theLexeme);
	return theToken;
	case 130:
	  theToken=tokenClass(ERROR_T, NONE_ST, theLexeme);
	  cerr << "Error:" << endl << "\t";
	  theFile.printCurrentLine();
	  cerr << "\tCarriage return in middle of a string literal." << endl;
	  return theToken;
	case 131:
	  theToken=tokenClass(ERROR_T, NONE_ST, theLexeme);
	  cerr << "Error:" << endl << "\t";
	  theFile.printCurrentLine();
	  cerr << "\tUnclosed string literal." << endl;
	  return theToken;
	case 132:
	  if (theLexeme.length() > 52) {
	    theToken=tokenClass(ERROR_T, NONE_ST, theLexeme);
	    cerr << "Error:" << endl << "\t";
	    theFile.printCurrentLine();
	    cerr << "\tString literal " << theLexeme.length() << " characters long." << endl;
	    return theToken;
	  }
	  theToken=tokenClass(STRING_T, NONE_ST, theLexeme);
	  return theToken;
	default:
	  continue;
    };
  } while (1);
  
  return theToken;
}

void scannerClass::printCurrentLine() {
  /**
   P *rints the current line by calling the FileManagerClass
   */
  theFile.printCurrentLine();
}

scannerClass::scannerClass() {
  /**
   I *nitializes a scannerClass by initializing theLexeme
   to an empty string.
   */
  theLexeme = "";
  expressionDebuggingStatus=false;
}

void scannerClass::tableAddLevel(string levelName, tableNodeClass* params) {
  //Method so that above classes can access the symbol table.
  //Adds a level to the symbol table.
  theTable.tableAddLevel(levelName,params);
}

void scannerClass::tableDeleteLevel() {
  //Method so that above classes can access the symbol table.
  //Deletes a level from the symbol table.
  theTable.tableDeleteLevel();
}

bool scannerClass::tableAddEntry(string lexeme, int kind, int type) {
  //Method so that above classes can access the symbol table.
  //Adds an entry to the top level of the symbol table.
  return theTable.tableAddEntry(lexeme, kind, type);
}

bool scannerClass::tableAddParam(string lexeme, int kind, int type) {
  //Method so that above classes can access the symbol table.
  //Adds a parameter to the last function in the symbol table.
  return theTable.tableAddParam(lexeme, kind, type);
}

tableNodeClass* scannerClass::tableLookup(string lex) {
  //Method so that above classes can access the symbol table.
  //Looks up a node in the symbol table.
  return theTable.tableLookup(lex);
}
void scannerClass::tableDeleteAllTemps() {
  //Method so that above classes can access the symbol table.
  //Delets all nodes with a temporary name ("$#", where # is a number)
  theTable.tableDeleteAllTemps();
}

void scannerClass::tableDeleteSpecialTemps() {
  //Method so that above classes can access the symbol table.
  //Delets all nodes with a special temp name ("$s#", where # is a number)
  theTable.tableDeleteSpecialTemps();
}