//Dan Fu, edited October 2012
//This file contains the implementations for the parser class

//The header is found in parser.h

#include "headers/parser.h"

string parseClass::getNextTemp() {
  //Generates the name of the next temporary variable.
  //For use when generating code during the expression parser.
  //May also be used when dealing with loops.
  static int nextCt = 0;
  char tempName[10];
  
  sprintf(tempName, "$%d", nextCt++);
  return tempName;
}

string parseClass::getNextSpecialTemp() {
  //Generates the name of the next special temp.
  //For use when parsing expressions within function calls.
  static int nextCt = 0;
  char tempName[10];
  
  sprintf(tempName, "$s%d", nextCt++);
  return tempName;
}

void parseClass::printInstruction(string instruction, tableNodeClass * ptr1, string str1, tableNodeClass * ptr2, string str2) {
  //Prints out a PAL instruction.
  //Instruction is: instruction ptr1Loc ptr2Loc
  //Where ptr1Loc and ptr2Loc are the locations of the variables
  //on the PAL stack.
  //If ptr1 or ptr2 is NULL, prints str1 or str2, respectively
  if (ptr1 != NULL) {
    outfile << "movw +";
    outfile << 4 * ptr1->nestingLevel;
    outfile << "$display R2" << endl;
  }
  if (ptr2 != NULL) {
    outfile << "movw +";
    outfile << 4 * ptr2->nestingLevel;
    outfile << "$display R3" << endl;
  }
  outfile << instruction << " ";
  if (ptr1 != NULL) {
    outfile << "+" << 4 * ptr1->offset;
    if (ptr1->kind==REF_PARAM) {
      outfile << "@";
    }
    outfile << "@R2";
  }
  else {
    outfile << str1;
  }
  outfile << " ";
  if (ptr2 != NULL) {
    outfile << "+" << 4 * ptr2->offset;
    if (ptr2->kind==REF_PARAM) {
      outfile << "@";
    }
    outfile << "@R3";
  }
  else {
    outfile << str2;
  }
  outfile << endl;
}

int parseClass::precedence(tokenClass token1, tokenClass token2) {
  /**
   *	Takes in two tokens and returns the precedence of the first one relative to
   *	the second one. It returns the precedence using integers defined in the header.
   *	It finds the precedence by using a 2D array and a precedence table.
   */
  
  int type1 = token1.getTypeNum();
  int type2 = token2.getTypeNum();
  
  int arrayNum1, arrayNum2; //The indices of type1 and type2
  
  //Assign arrayNum1 and arrayNum2
  //relop=0,addop=1,mulop=2,(=3,)=4,id=5,ic=6,true=7,false=8,;=9,other=10
  switch(type1) {
    case RELOP_T:
      arrayNum1=0;
      break;
    case ADDOP_T:
      arrayNum1=1;
      break;
    case MULOP_T:
      arrayNum1=2;
      break;
    case LPAR_T:
      arrayNum1=3;
      break;
    case RPAR_T:
      arrayNum1=4;
      break;
    case IDENTIFIER_T:
      arrayNum1=5;
      break;
    case INTEGER_T:
      arrayNum1=6;
      break;
    case TRUE_T:
      arrayNum1=7;
      break;
    case FALSE_T:
      arrayNum1=8;
      break;
    case SEMICOLON_T:
      arrayNum1=9;
      break;
    default:
      arrayNum1=10;
  }
  
  switch(type2) {
    case RELOP_T:
      arrayNum2=0;
      break;
    case ADDOP_T:
      arrayNum2=1;
      break;
    case MULOP_T:
      arrayNum2=2;
      break;
    case LPAR_T:
      arrayNum2=3;
      break;
    case RPAR_T:
      arrayNum2=4;
      break;
    case IDENTIFIER_T:
      arrayNum2=5;
      break;
    case INTEGER_T:
      arrayNum2=6;
      break;
    case TRUE_T:
      arrayNum2=7;
      break;
    case FALSE_T:
      arrayNum2=8;
      break;
    case SEMICOLON_T:
      arrayNum2=9;
      break;
    case DO_T:
      arrayNum2=9;
      break;
    case THEN_T:
      arrayNum2=9;
      break;
    case ELSE_T:
      arrayNum2=9;
      break;
    case COMMA_T:
      arrayNum2=9;
      break;
    case END_T:
      arrayNum2=9;
      break;
      /**case RPAR_T:
       *			arrayNum2=9;
       *			break;*/
      case LL_T:
	arrayNum2=9;
	break;
      default:
	arrayNum2=10;
  }
  
  //Create precedence table
  //precTable[i][j] means row i, column j, token1 is i, token2 is j
  static int precTable [11][11] = {
    {GT, LT, LT, LT, GT, LT, LT, LT, LT, GT, ER}, //relop=0
    {GT, GT, LT, LT, GT, LT, LT, LT, LT, GT, ER}, //addop=1
    {GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, ER}, //mulop=2
    {LT, LT, LT, LT, EQ, LT, LT, LT, LT, ER, ER}, //(=3
    {GT, GT, GT, ER, GT, ER, ER, ER, ER, GT, ER}, //)=4
    {GT, GT, GT, ER, GT, ER, ER, ER, ER, GT, ER}, //id=5
    {GT, GT, GT, ER, GT, ER, ER, ER, ER, GT, ER}, //ic=6
    {GT, GT, GT, ER, GT, ER, ER, ER, ER, GT, ER}, //true=7
    {GT, GT, GT, ER, GT, ER, ER, ER, ER, GT, ER}, //false=8
    {LT, LT, LT, LT, ER, LT, LT, LT, LT, ER, ER}, //semicolon=9
    {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER} //other=10
  };
  
  return precTable[arrayNum1][arrayNum2];
}

string parseClass::validRHS(tokenClass symbolsPopped[3], int arraySize) {
  /**
   *	Tries to turn an array into a valid right hand side of E -> ... expression.
   *	@param symbolsPopped the array that may or may not contain a valid RHS.
   *	@param arraySize the number of terms in the array.
   *	@return "NO" if the array does not contain a valid RHS, the appropriate
   *	E -> ... expression if it does.
   */
  if (arraySize==0) {
    return "NO";
  }
  else if (arraySize==1) {
    int type = symbolsPopped[0].getTypeNum();
    switch(type) {
      case INTEGER_T:
	return "E -> integer-constant";
      case IDENTIFIER_T:
	return "E -> id";
      case TRUE_T:
	return "E -> true";
      case FALSE_T:
	return "E -> false";
      default:
	return "NO";
    }
  }
  else if (arraySize==2) {
    return "NO";
  }
  else if (arraySize==3) {
    int type0 = symbolsPopped[0].getTypeNum();
    if (type0==RPAR_T) {
      if (symbolsPopped[1].getTypeNum() == E_T && symbolsPopped[2].getTypeNum() == LPAR_T) {
	return "E -> (E)";
      }
      else {
	return "NO";
      }
    }
    else if (type0==E_T) {
      if(symbolsPopped[2].getTypeNum()!=E_T) {
	return "NO";
      }
      int type1=symbolsPopped[1].getTypeNum();
      int subType1=symbolsPopped[1].getSubtypeNum();
      if (type1==ADDOP_T) {
	switch(subType1) {
	  case PLUS_ST:
	    return "E -> E + E";
	  case MINUS_ST:
	    return "E -> E - E";
	  case OR_ST:
	    return "E -> E or E";
	  default:
	    return "NO";
	}
      }
      else if (type1==MULOP_T) {
	switch(subType1) {
	  case AND_ST:
	    return "E -> E and E";
	  case ASTERISK_ST:
	    return "E -> E * E";
	  case DIV_ST:
	    return "E -> E div E";
	  case MOD_ST:
	    return "E -> E mod E";
	  default:
	    return "NO";
	}
      }
      else if (type1==RELOP_T) {
	switch(subType1) {
	  case LT_ST:
	    return "E -> E < E";
	  case LEQ_ST:
	    return "E -> E <= E";
	  case EQUAL_ST:
	    return "E -> E == E";
	  case GEQ_ST:
	    return "E -> E >= E";
	  case GT_ST:
	    return "E -> E > E";
	  case NEQ_ST:
	    return "E -> E <> E";
	  default:
	    return "NO";
	}
      }
      else {
	return "NO";
      }
    }
    else {
      return "NO";
    }
  }
  else {
    return "NO";
  }
}

void parseClass::parse_expr() {
  /**
   *	The main method of the parseClass for expressions.
   *	Uses operator-precedence parsing to parse through an expression.
   */
  printInstruction("movw", NULL, "SP", NULL, "R1");
  
  int arraySize=0;
  tokenClass symbolsPopped [3];
  pStackClass expression;
  tokenClass stackTop (SEMICOLON_T, NONE_ST, "");
  expression.push(stackTop);
  tokenClass topTerm;
  lastReduced = NULL;
  //Check to make sure the first token is in the first set of an expression
  if (token.getTypeNum() != LPAR_T &&
    token.getTypeNum() != INTEGER_T &&
    token.getTypeNum() != TRUE_T &&
    token.getTypeNum() != FALSE_T &&
    token.getTypeNum() != IDENTIFIER_T) {
    cerr << "Error: bad expression at or near line:" << endl << "\t";
  scanner.printCurrentLine();
  cin.get();
  exit(1);
    }
    while (true) {
      tokenClass topTermTemp = expression.returnTopMostTerminal();
      int tokenType = token.getTypeNum();
      if ((tokenType == SEMICOLON_T ||
	tokenType == DO_T ||
	tokenType == THEN_T ||
	tokenType == ELSE_T ||
	tokenType == COMMA_T ||
	tokenType == END_T ||
	tokenType == RPAR_T ||
	tokenType == LL_T)
	&& topTermTemp.getTypeNum() == SEMICOLON_T) {
	if (lastReduced != NULL) {
	  printInstruction("movw", lastReduced, "", NULL, "@R1");
	}
	printInstruction("movw", NULL, "R1", NULL, "SP");
	printInstruction("addw", NULL, "#4", NULL, "SP");
	scanner.tableDeleteAllTemps();
	return;
	}
	else {
	  if (tokenType == IDENTIFIER_T) {
	    string lexeme = token.getLexeme();
	    token.symTableVariable = scanner.tableLookup(lexeme);
	    if (token.symTableVariable == NULL) {
	      undecVarError(lexeme);
	    }
	  }
	  else if (tokenType == INTEGER_T) {
	    int value = atoi(token.getLexeme().c_str());
	    string tempConst = getNextTemp();
	    scanner.tableAddEntry(tempConst, VAR_ID, INT_TYPE);
	    token.symTableVariable = scanner.tableLookup(tempConst);
	    char PALVal[10];
	    sprintf(PALVal, "#%d", value);
	    printInstruction("movw", NULL, PALVal, token.symTableVariable, "");
	    printInstruction("addw", NULL, "#4", NULL, "SP");
	  }
	  else if (tokenType == TRUE_T || tokenType == FALSE_T) {
	    string tempConst = getNextTemp();
	    scanner.tableAddEntry(tempConst, VAR_ID, BOOLEAN_TYPE);
	    token.symTableVariable = scanner.tableLookup(tempConst);
	    string value = "#0";
	    if (tokenType == TRUE_T) {
	      value = "#1";
	    }
	    printInstruction("movw", NULL, value, token.symTableVariable, "");
	    printInstruction("addw", NULL, "#4", NULL, "SP");
	  }
	  topTerm=topTermTemp;
	  int precTopAndToken=precedence(topTerm,token);
	  if (precTopAndToken==LT || precTopAndToken==EQ) {
	    expression.push(token);
	    lastReduced = token.symTableVariable;
	    token = scanner.getNextToken();
	  }
	  else if (precTopAndToken==GT) {
	    do {
	      symbolsPopped[arraySize]=expression.pop();
	      arraySize++;
	    } while (arraySize < 3 &&
	    (arraySize==0 ||
	    !expression.isThereATerminalOnTop() ||
	    precedence(expression.returnTopMostTerminal(),symbolsPopped[arraySize-1])!=LT));
	    string rightHandSide=validRHS(symbolsPopped, arraySize);
	    if (strcmp(rightHandSide.c_str(),"NO")!=0) {
	      if (scanner.expressionDebuggingStatus) {
		cout << rightHandSide << endl;
	      }
	      tokenClass toPush (E_T, NONE_ST, "");
	      if (arraySize == 1) {
		toPush.symTableVariable = symbolsPopped[0].symTableVariable;
	      }
	      else if (symbolsPopped[0].getTypeNum()==RPAR_T) {
		toPush.symTableVariable = symbolsPopped[1].symTableVariable;
	      }
	      else {
		//Semantic Analysis
		int operation = symbolsPopped[1].getSubtypeNum();
		if (operation == PLUS_ST || operation == MINUS_ST ||
		  operation == DIV_ST || operation == ASTERISK_ST ||
		  operation == MOD_ST) {
		  if (symbolsPopped[0].symTableVariable->type != INT_TYPE ||
		    symbolsPopped[2].symTableVariable->type != INT_TYPE) {
		    if (symbolsPopped[0].symTableVariable->type == BOOLEAN_TYPE ||
		      symbolsPopped[2].symTableVariable->type == BOOLEAN_TYPE) {
		      errorAndQuit("boolean_Variable", "< integer >");
		      }
		      else {
			errorAndQuit("non-integer identifier", "< integer >"); 
		      }
		    }
		  }
		  else if (operation == AND_ST || operation == OR_ST) {
		    if (symbolsPopped[0].symTableVariable->type != BOOLEAN_TYPE ||
		      symbolsPopped[2].symTableVariable->type != BOOLEAN_TYPE) {
		      if (symbolsPopped[0].symTableVariable->type == INT_TYPE ||
			symbolsPopped[2].symTableVariable->type == INT_TYPE) {
			errorAndQuit("integer_Variable", "< boolean >");
			}
			else {
			  errorAndQuit("non-boolean identifier", "< boolean >"); 
			}
		      }
		  }
		  else if (operation == EQUAL_ST || operation == NEQ_ST || operation == LT_ST ||
		    operation == LEQ_ST || operation == GEQ_ST || operation == GT_ST) {
		    if (symbolsPopped[0].symTableVariable->type !=
		      symbolsPopped[2].symTableVariable->type) {
		      if (symbolsPopped[2].symTableVariable->type == INT_TYPE) {
			errorAndQuit("boolean_Expression",
				     "< integer_Expression >");
		      }
		      else {
			errorAndQuit("integer_Expression",
				     "< boolean_Expression >");
		      }
		      }
		    }
		    string newTempName = getNextTemp();
		    bool int_Simple = false;
		    string opName = "";
		    bool int_Mod = false;
		    bool boolean_Simple = false;
		    if (symbolsPopped[1].getSubtypeNum()==PLUS_ST) {
		      int_Simple = true;
		      opName = "addw";
		    }
		    else if (symbolsPopped[1].getSubtypeNum()==MINUS_ST) {
		      int_Simple = true;
		      opName = "subw";
		    }
		    else if (symbolsPopped[1].getSubtypeNum()==ASTERISK_ST) {
		      int_Simple = true;
		      opName = "mulw";
		    }
		    else if (symbolsPopped[1].getSubtypeNum()==DIV_ST) {
		      int_Simple = true;
		      opName = "divw";
		    }
		    else if (symbolsPopped[1].getSubtypeNum()==MOD_ST) {
		      int_Mod = true;
		    }
		    else if (symbolsPopped[1].getSubtypeNum()==EQUAL_ST) {
		      boolean_Simple = true;
		      opName = "beq";
		    }
		    else if (symbolsPopped[1].getSubtypeNum()==NEQ_ST) {
		      boolean_Simple = true;
		      opName = "bneq";
		    }
		    else if (symbolsPopped[1].getSubtypeNum()==LT_ST) {
		      boolean_Simple = true;
		      opName = "blss";
		    }
		    else if (symbolsPopped[1].getSubtypeNum()==LEQ_ST) {
		      boolean_Simple = true;
		      opName = "bleq";
		    }
		    else if (symbolsPopped[1].getSubtypeNum()==GT_ST) {
		      boolean_Simple = true;
		      opName = "bgtr";
		    }
		    else if (symbolsPopped[1].getSubtypeNum()==GEQ_ST) {
		      boolean_Simple = true;
		      opName = "bgeq";
		    }
		    if (int_Simple) {
		      scanner.tableAddEntry(newTempName, VAR_ID, INT_TYPE);
		      tableNodeClass * newEntry = scanner.tableLookup(newTempName);
		      printInstruction("addw", NULL, "#4", NULL, "SP");
		      printInstruction("movw", symbolsPopped[2].symTableVariable, "", newEntry, "");
		      printInstruction(opName, symbolsPopped[0].symTableVariable, "", newEntry, "");
		      toPush.symTableVariable = newEntry;
		    }
		    else if (int_Mod) {
		      scanner.tableAddEntry(newTempName, VAR_ID, INT_TYPE);
		      tableNodeClass * temp1 = scanner.tableLookup(newTempName);
		      string temp2Name = getNextTemp();
		      string temp3Name = getNextTemp();
		      scanner.tableAddEntry(temp2Name, VAR_ID, INT_TYPE);
		      tableNodeClass * temp2 = scanner.tableLookup(temp2Name);
		      scanner.tableAddEntry(temp3Name, VAR_ID, INT_TYPE);
		      tableNodeClass * temp3 = scanner.tableLookup(temp3Name);
		      printInstruction("addw", NULL, "#12", NULL, "SP");
		      printInstruction("movw", symbolsPopped[2].symTableVariable, "", temp1, "");
		      printInstruction("divw", symbolsPopped[0].symTableVariable, "", temp1, "");
		      printInstruction("movw", temp1, "", temp2, "");
		      printInstruction("mulw", symbolsPopped[0].symTableVariable, "", temp2, "");
		      printInstruction("movw", symbolsPopped[2].symTableVariable, "", temp3, "");
		      printInstruction("subw", temp2, "", temp3, "");
		      toPush.symTableVariable = temp3;
		    }
		    else if (symbolsPopped[1].getSubtypeNum()==OR_ST) {
		      scanner.tableAddEntry(newTempName, VAR_ID, BOOLEAN_TYPE);
		      tableNodeClass * newEntry = scanner.tableLookup(newTempName);
		      printInstruction("addw", NULL, "#4", NULL, "SP");
		      printInstruction("movw", symbolsPopped[2].symTableVariable, "", newEntry, "");
		      printInstruction("addw", symbolsPopped[0].symTableVariable, "", newEntry, "");
		      printInstruction("cmpw", NULL, "#0", newEntry, "");
		      string nextLabel = getNextTemp();
		      printInstruction("beq", NULL, nextLabel, NULL, "");
		      printInstruction("movw", NULL, "#1", newEntry, "");
		      outfile << nextLabel << " movw R0 R0" << endl;
		      toPush.symTableVariable = newEntry;
		    }
		    else if (symbolsPopped[1].getSubtypeNum()==AND_ST) {
		      scanner.tableAddEntry(newTempName, VAR_ID, BOOLEAN_TYPE);
		      tableNodeClass * newEntry = scanner.tableLookup(newTempName);
		      printInstruction("addw", NULL, "#4", NULL, "SP");
		      printInstruction("movw", symbolsPopped[2].symTableVariable, "", newEntry, "");
		      printInstruction("mulw", symbolsPopped[0].symTableVariable, "", newEntry, "");
		      toPush.symTableVariable = newEntry;
		    }
		    else if (boolean_Simple) {
		      scanner.tableAddEntry(newTempName, VAR_ID, BOOLEAN_TYPE);
		      tableNodeClass * newEntry = scanner.tableLookup(newTempName);
		      printInstruction("addw", NULL, "#4", NULL, "SP");
		      printInstruction("cmpw", symbolsPopped[2].symTableVariable, "", symbolsPopped[0].symTableVariable, "");
		      string nextLabel = getNextTemp();
		      printInstruction("movw", NULL, "#1", newEntry, "");
		      printInstruction(opName, NULL, nextLabel, NULL, "");
		      printInstruction("movw", NULL, "#0", newEntry, "");
		      outfile << nextLabel << " movw R0 R0" << endl;
		      toPush.symTableVariable = newEntry;
		    }
	      }
	      arraySize=0;
	      expression.push(toPush);
	      lastReduced = toPush.symTableVariable;
	    }
	    else {
	      cerr << "Syntax Error: Invalid Expression" << endl << "\t";
	      scanner.printCurrentLine();
	      cin.get();
	      exit(1);
	    }
	  }
	  else {
	    cerr << "Syntax Error: Invalid Expression" << endl << "\t";
	    scanner.printCurrentLine();
	    cin.get();
	    exit(1);
	  }
	}
    }
}

parseClass::parseClass() {
  /**
   *	The constructor for the parseClass. Open the output file.
   */
  outfile.open("out.pal",ios::out);
  currentNestingLevel=-1;
  highestNestingLevel=0;
}

void parseClass::parseProgram() {
  /**
   *	Parses the entire program.
   */
  token=scanner.getNextToken();
  program();
  outfile.close();
}

void parseClass::statement() {
  /**
   *	Parses a statement.  If there is not a statement, throws an error message.
   */
  int tokenType=token.getTypeNum();
  if (tokenType == WHILE_T) {
    string topName = getNextTemp();
    string bottomName = getNextTemp();
    printInstruction(topName + " movw", NULL, "R0", NULL, "R0 :Dummy, while loop top");
    token=scanner.getNextToken();
    parse_expr();
    if (token.getTypeNum() == DO_T) {
      if (lastReduced->type!=BOOLEAN_TYPE) {
	errorAndQuit("integer_Expression", "< boolean_Expression >");
      }
      printInstruction("cmpw", NULL, "#0", NULL, "-4@SP");
      printInstruction("subw", NULL, "#4", NULL, "SP");
      printInstruction("beq", NULL, bottomName, NULL, "");
      token=scanner.getNextToken();
      statement();
      printInstruction("jmp", NULL, topName, NULL, "");
      printInstruction(bottomName + " movw", NULL, "R0", NULL, "R0 :Dummy, while loop bottom");
    }
    else {
      errorAndQuit(token.getLexeme(),"< do >");
    }
  }
  else if (tokenType == IF_T) {
    token=scanner.getNextToken();
    parse_expr();
    if (token.getTypeNum() == THEN_T) {
      if (lastReduced->type!=BOOLEAN_TYPE) {
	errorAndQuit("integer_Expression", "< boolean_Expression >");
      }
      string elseLabel = getNextTemp();
      string afterIfLabel = getNextTemp();
      printInstruction("cmpw", NULL, "#0", NULL, "-4@SP");
      printInstruction("subw", NULL, "#4", NULL, "SP");
      printInstruction("beq", NULL, elseLabel, NULL, "");
      token=scanner.getNextToken();
      statement();
      followIf(elseLabel, afterIfLabel);
    }
    else {
      errorAndQuit(token.getLexeme(),"< then >");
    }
  }
  else if (tokenType == IDENTIFIER_T) {
    tableNodeClass *ident=scanner.tableLookup(token.getLexeme());
    token=scanner.getNextToken();
    if (ident==NULL) {
      undecVarError(token.getLexeme());
    }
    followId(ident);
  }
  else if (tokenType == COUT_T) {
    token=scanner.getNextToken();
    followCout();
  }
  else if (tokenType == CIN_T) {
    token=scanner.getNextToken();
    followCin();
  }
  else if (tokenType == BEGIN_T) {
    token=scanner.getNextToken();
    followBegin();
  }
  else {
    errorAndQuit(token.getLexeme(),"< while, if, identifier, cout, cin, begin >");
  }
  /*else if (token.getTypeNum() == IDENTIFIER_T) {
   *	token=scanner.getNextToken();
   *	if (token.getTypeNum() == ASSIGNMENT_T) {
   *	token=scanner.getNextToken();
   *	parse_expr();
   *	if (token.getTypeNum() == SEMICOLON_T) {
   *	token=scanner.getNextToken();
}
else {
  errorAndQuit(token.getLexeme(),"<;>");
}
}
else {
  errorAndQuit(token.getLexeme(),"<=>");
}
}
else {
  errorAndQuit(token.getLexeme(),"<while, identifier>");
}*/
}

void parseClass::followBegin() {
  /**
   *	Parses something that follows a begin token.
   */
  int tokenType = token.getTypeNum();
  if (tokenType == END_T) {
    token=scanner.getNextToken();
    return;
  }
  else {
    statement();
    statementTail();
    if (token.getTypeNum() == END_T) {
      token=scanner.getNextToken();
      return;
    }
    else {
      errorAndQuit(token.getLexeme(),"< end >");
    }
  }
}

void parseClass::statementTail() {
  /**
   *	Parses something that can come after a statement.
   */
  int tokenType = token.getTypeNum();
  if (tokenType == SEMICOLON_T) {
    token=scanner.getNextToken();
    statement();
    statementTail();
  }
  else if (tokenType==END_T) {
    return;
  }
}

void parseClass::followIf(string elseLabel, string afterIfLabel) {
  /**
   *	Parses something that can follow an if token.
   */
  int tokenType = token.getTypeNum();
  if (tokenType == ELSE_T) {
    token=scanner.getNextToken();
    printInstruction("jmp", NULL, afterIfLabel, NULL, "");
    printInstruction(elseLabel + " movw", NULL, "R0", NULL, "R0 :Dummy, else label");
    statement();
    printInstruction(afterIfLabel + " movw", NULL, "R0", NULL, "R0 :Dummy, after if label");
  }
  else if (tokenType == SEMICOLON_T ||
    tokenType == END_T) {
    printInstruction(elseLabel + " movw", NULL, "R0", NULL, "R0 :Dummy, else label");
  return;
    }
    else {
      errorAndQuit(token.getLexeme(),"< else, ;, end >");
    }
}

void parseClass::followId(tableNodeClass *ident) {
  /**
   *	Parses something that can follow an identifier.
   */
  int tokenType = token.getTypeNum();
  if (tokenType == ASSIGNMENT_T) {
    token=scanner.getNextToken();
    parse_expr();
    if (ident->kind != VAR_ID && ident->kind != VALUE_PARAM && ident->kind != REF_PARAM) {
      errorAndQuit("non-variable", "< variable >");
    }
    if (ident->type != lastReduced->type) {
      if (ident->type==INT_TYPE) {
	errorAndQuit("boolean_Expression", "< integer_Expression >");
      }
      else {
	errorAndQuit("integer_Expression", "< boolean_Expression >");
      }
    }
    printInstruction("movw", NULL, "-4@SP", ident, "");
    printInstruction("subw", NULL, "#4", NULL, "SP");
  }
  else if (tokenType == GL_T) {
    token=scanner.getNextToken();
    if (token.getTypeNum() == IDENTIFIER_T) {
      tableNodeClass * ident2 = scanner.tableLookup(token.getLexeme());
      if (ident==NULL) {
	undecVarError(token.getLexeme());
      }
      if (ident->kind != VAR_ID || ident2->kind != VAR_ID) {
	errorAndQuit("non-variable", "< variable >");
      }
      if (ident->type != ident2->type) {
	if (ident->type==INT_TYPE) {
	  errorAndQuit("boolean_Variable", "< integer_Variable >");
	}
	else {
	  errorAndQuit("integer_Variable", "< boolean_Variable >");
	}
      }
      string newTempName = getNextTemp();
      scanner.tableAddEntry(newTempName, VAR_ID, ident->type);
      tableNodeClass * newEntry = scanner.tableLookup(newTempName);
      printInstruction("movw", NULL, "#4", NULL, "SP");
      printInstruction("movw", ident, "", newEntry, "");
      printInstruction("movw", ident2, "", ident, "");
      printInstruction("movw", newEntry, "", ident2, "");
      token=scanner.getNextToken();
      return;
    }
    else {
      errorAndQuit(token.getLexeme(),"< identifier >");
    }
  }
  else if (tokenType == LPAR_T) {
    token=scanner.getNextToken();
    tableNodeClass *params = ident->params->next;
    if (params==NULL) {
      numParamError();
    }
    string specTempName = getNextSpecialTemp();
    if (params->kind==VALUE_PARAM) {
      parse_expr();
      scanner.tableAddEntry(specTempName, VALUE_PARAM, lastReduced->type);
    }
    else {
      if (token.getTypeNum()!=IDENTIFIER_T) {
	errorAndQuit("expression", "< variable_Name >");
      }
      string lexeme = token.getLexeme();
      parse_expr();
      printInstruction("subw", NULL, "#4", NULL, "SP");
      printInstruction("mova", scanner.tableLookup(lexeme), "", NULL, "@SP");
      printInstruction("addw", NULL, "#4", NULL, "SP");
      scanner.tableAddEntry(specTempName, REF_PARAM, lastReduced->type);
    }
    if (lastReduced->type!=params->type) {
      if (lastReduced->type==INT_TYPE) {
	errorAndQuit("integer_Expression", "< boolean_Expression >");
      }
      else {
	errorAndQuit("boolean_Expression", "< integer_Expression >");
      }
    }
    params=params->next;
    int numParams = 1 + followExpression(params);
    if (token.getTypeNum()==RPAR_T) {
      token=scanner.getNextToken();
      scanner.tableDeleteSpecialTemps();
      char numParamsString [10];
      sprintf(numParamsString, "#%d", numParams);
      char funcName [10];
      sprintf(funcName, "$%d", ident->offset);
      printInstruction("call", NULL, numParamsString, NULL, funcName);
      return;
    }
    else {
      errorAndQuit(token.getLexeme(),"< ) >");
    }
  }
  else if (tokenType == ELSE_T ||	tokenType == SEMICOLON_T ||
    tokenType == END_T) {
    if (ident->params->next!=NULL) {
      numParamError();
    }
    char funcLabel[10];
  sprintf(funcLabel, "$%d", ident->offset);
  printInstruction("call", NULL, "#0", NULL, funcLabel);
  return;
    }
    else {
      errorAndQuit(token.getLexeme(),"< else, ;, end >");
    }
}

int parseClass::followExpression(tableNodeClass *params) {
  /**
   *	Parses something that can follow an expression (in a list of parameters in a function call).
   */
  int tokenType = token.getTypeNum();
  if (tokenType == COMMA_T) {
    token=scanner.getNextToken();
    if (params==NULL) {
      numParamError();
    }
    string specTempName = getNextSpecialTemp();
    if (params->kind==VALUE_PARAM) {
      parse_expr();
      scanner.tableAddEntry(specTempName, VALUE_PARAM, lastReduced->type);
    }
    else {
      if (token.getTypeNum()!=IDENTIFIER_T) {
	errorAndQuit("expression", "< variable_Name >");
      }
      string lexeme = token.getLexeme();
      parse_expr();
      printInstruction("subw", NULL, "#4", NULL, "SP");
      printInstruction("mova", scanner.tableLookup(lexeme), "", NULL, "@SP");
      printInstruction("addw", NULL, "#4", NULL, "SP");
      scanner.tableAddEntry(specTempName, REF_PARAM, lastReduced->type);
    }
    if (lastReduced->type!=params->type) {
      if (lastReduced->type==INT_TYPE) {
	errorAndQuit("integer_Expression", "< boolean_Expression >");
      }
      else {
	errorAndQuit("boolean_Expression", "< integer_Expression >");
      }
    }
    params=params->next;
    return 1 + followExpression(params);
  }
  else if (tokenType == RPAR_T) {
    if (params!=NULL) {
      numParamError();
    }
    return 0;
  }
  else {
    errorAndQuit(token.getLexeme(),"< ,, ) >");
  }
}

void parseClass::followCin() {
  /**
   *	Parses something that can follow a cin token.
   */
  int tokenType = token.getTypeNum();
  if (tokenType == GG_T) {
    token=scanner.getNextToken();
    if (token.getTypeNum() == IDENTIFIER_T) {
      string curLexeme = token.getLexeme();
      tableNodeClass * variable = scanner.tableLookup(curLexeme);
      if (variable->type == BOOLEAN_TYPE) {
	errorAndQuit("boolean_Variable", "< integer_Variable >");
      }
      printInstruction("inw", variable, "", NULL, "");
      token=scanner.getNextToken();
      return;
    }
    else {
      errorAndQuit(token.getLexeme(),"< identifier >");
    }
  }
  else if (tokenType == ELSE_T ||
    tokenType == SEMICOLON_T ||
    tokenType == END_T) {
    outfile << "inb @sp" << endl;
  return;
    }
    else {
      errorAndQuit(token.getLexeme(),"< >>, else, ;, end >");
    }
}

void parseClass::followCout() {
  /**
   *	Parses something that can follow a cout token.
   */
  int tokenType = token.getTypeNum();
  if (tokenType == LL_T) {
    token=scanner.getNextToken();
    ffCout();
  }
  else {
    errorAndQuit(token.getLexeme(),"< << >");
  }
}

void parseClass::ffCout() {
  /**
   *	Parses something that can follow followCout.
   */
  int tokenType = token.getTypeNum();
  if (tokenType == STRING_T) {
    string toPrint = token.getLexeme();
    for (int i = 1; i < toPrint.length()-1; i++) {
      if (toPrint[i] == ' ') {
	printInstruction("outb", NULL, "#32", NULL, "");
      }
      else {
	outfile << "outb ^" << toPrint.c_str()[i] << endl;
      }
    }
    token=scanner.getNextToken();
    fffCout();
  }
  else if (tokenType == ENDL_T) {
    printInstruction("outb", NULL, "#10", NULL, "");
    token=scanner.getNextToken();
    fffCout();
  }
  else {
    parse_expr();
    printInstruction("outw", NULL, "@R1", NULL, "");
    printInstruction("subw", NULL, "#4", NULL, "SP");
    fffCout();
  }
}

void parseClass::fffCout() {
  /**
   *	Parses something that can follow ffCout.
   */
  int tokenType = token.getTypeNum();
  if (tokenType == LL_T) {
    token = scanner.getNextToken();
    ffCout();
  }
  else if (tokenType == ELSE_T ||
    tokenType == SEMICOLON_T ||
    tokenType == END_T) {
    return;
    }
    else {
      errorAndQuit(token.getLexeme(), "< <<, else, ;, end >");
    }
}

void parseClass::program() {
  /**
   *	Parses a full program.
   */
  int tokenType = token.getTypeNum();
  if (tokenType == PROGRAM_T) {
    outfile << "$main movw SP R0" << endl;
    token=scanner.getNextToken();
    if (token.getTypeNum() == IDENTIFIER_T) {
      token=scanner.getNextToken();
      if (token.getTypeNum() == SEMICOLON_T) {
	token=scanner.getNextToken();
	block("main", NULL);
	if (token.getTypeNum() == PERIOD_T) {
	  outfile << "inb @sp" << endl;
	  outfile << "end" << endl;
	  token=scanner.getNextToken();
	  return;
	}
	else {
	  errorAndQuit(token.getLexeme(),"< . >");
	}
      }
      else {
	errorAndQuit(token.getLexeme(), "< ; >");
      }
    }
    else {
      errorAndQuit(token.getLexeme(),"< identifier >");
    }
  }
  else {
    errorAndQuit(token.getLexeme(),"< program >");
  }
}

int parseClass::type() {
  /**
   *	Parses a type (int or boolean).
   */
  int tokenType = token.getTypeNum();
  if (tokenType == INT_T) {
    token=scanner.getNextToken();
    return INT_T;
  }
  else if (tokenType == BOOLEAN_T) {
    token=scanner.getNextToken();
    return BOOLEAN_T;
  }
  else {
    errorAndQuit(token.getLexeme(),"< int, boolean >");
    return 0;
  }
}

void parseClass::block(string name, tableNodeClass *params) {
  /**
   *	Parses a block (e.g. what follows the program declaration and what comes before the period).
   */
  scanner.tableAddLevel(name, params);
  currentNestingLevel++;
  if (currentNestingLevel > highestNestingLevel) {
    highestNestingLevel = currentNestingLevel;
  }
  char displayReference[20];
  sprintf(displayReference, "+%d$display", currentNestingLevel*4);
  if (currentNestingLevel != 0) {
    printInstruction("movw", NULL, displayReference, NULL, "+0@FP");
    printInstruction("movw", NULL, "FP", NULL, displayReference);
  }
  string bodyLabel = getNextTemp();
  varDecs(0);
  printInstruction("jmp", NULL, bodyLabel, NULL, "");
  funcDecs();
  char displayMemory [10];
  sprintf(displayMemory, "#%d", 4 * (highestNestingLevel + 1));
  if (currentNestingLevel==0) {
    printInstruction("$display", NULL, displayMemory, NULL, "");
  }
  printInstruction(bodyLabel + " movw", NULL, "R0", NULL, "R0");
  if (currentNestingLevel==0) {
    printInstruction("movw", NULL, "R0", NULL, displayReference);
  }
  int paramCount = 0;
  while (params!=NULL) {
    params=params->next;
    paramCount++;
  }
  for (int i = 1; i <= paramCount; i++) {
    int APOffset = (paramCount - i)*4;
    int FPOffset = (i-1)*4;
    char APString [15];
    char FPString [15];
    sprintf(APString, "-%d@AP", APOffset);
    sprintf(FPString, "+%d@FP", FPOffset);
    if (i != 1) {
      printInstruction("movw", NULL, APString, NULL, FPString);
    }
    printInstruction("addw", NULL, "#4", NULL, "SP");
  }
  progBody();
  if (currentNestingLevel != 0) {
    printInstruction("movw", NULL, "+0@FP", NULL, displayReference);
  }
  scanner.tableDeleteLevel();
  currentNestingLevel--;
}

void parseClass::progBody() {
  /**
   *	Parses the boyd of a block (what the actual "actions" taken are).
   */
  int tokenType = token.getTypeNum();
  if (tokenType == BEGIN_T) {
    token=scanner.getNextToken();
    followBegin();
  }
  else {
    errorAndQuit(token.getLexeme(),"< begin >");
  }
}

void parseClass::varDecs(int numVars) {
  /**
   *	Parses a series of variable declarations.
   */
  int tokenType = token.getTypeNum();
  if (tokenType == INT_T || tokenType == BOOLEAN_T) {
    type();
    int identType = 0;
    if (tokenType == INT_T) { identType = INT_TYPE; }
    else if (tokenType == BOOLEAN_T) { identType = BOOLEAN_TYPE; }
    numVars += identList(identType);
    if (token.getTypeNum() == SEMICOLON_T) {
      token=scanner.getNextToken();
      varDecs(numVars);
    }
    else {
      errorAndQuit(token.getLexeme(),"< ; >");
    }
  }
  else if (tokenType == FUNCTION_T || tokenType == BEGIN_T) {
    char varSpace [10];
    sprintf(varSpace, "#%d", 4 * numVars);
    printInstruction("addw", NULL, varSpace, NULL, "SP");
    return;
  }
  else {
    errorAndQuit(token.getLexeme(),"< int, boolean, follow, begin >");
  }
}

int parseClass::identList(int identType) {
  /**
   *	Parses a list of identifiers in a list of variable delcarations.
   */
  if (token.getTypeNum() == IDENTIFIER_T) {
    if (!scanner.tableAddEntry(token.getLexeme(), VAR_ID, identType)) {
      duplicateIdentError(token.getLexeme());
    }
    token=scanner.getNextToken();
    return 1 + identTailIdentList(identType);
  }
  else {
    errorAndQuit(token.getLexeme(),"< identifier >");
    return 0;
  }
}

int parseClass::identTailIdentList(int identType) {
  /**
   *	Parses something that can an identifier in a series of variable declarations.
   */
  int tokenType = token.getTypeNum();
  if (tokenType == COMMA_T) {
    token=scanner.getNextToken();
    return identList(identType);
  }
  else if (tokenType == SEMICOLON_T) {
    return 0;
  }
  else {
    errorAndQuit(token.getLexeme(), "< , , ; >");
    return 0;
  }
}

void parseClass::funcDecs() {
  /**
   *	Parses a series of function declarations.
   */
  int tokenType = token.getTypeNum();
  if (tokenType == FUNCTION_T) {
    token=scanner.getNextToken();
    if (token.getTypeNum() == IDENTIFIER_T) {
      if (!scanner.tableAddEntry(token.getLexeme(), FUNC_ID, FUNC_ID_TYPE)) {
	duplicateIdentError(token.getLexeme());
      }
      string functionName=token.getLexeme();
      tableNodeClass * func = scanner.tableLookup(functionName);
      string functionLabel = getNextTemp();
      func->offset=atoi(functionLabel.substr(1,functionLabel.length()-1).c_str());
      printInstruction(functionLabel + " movw", NULL, "R0", NULL, "R0");
      token=scanner.getNextToken();
      scanner.tableAddParam("$$", VALUE_PARAM, INT_TYPE);
      funcDecTail();
      if (token.getTypeNum() == SEMICOLON_T) {
	token=scanner.getNextToken();
	tableNodeClass *param = scanner.tableLookup(functionName)->params;
	block(functionName, param);
	if (token.getTypeNum() == SEMICOLON_T) {
	  printInstruction("movw", NULL, "FP", NULL, "SP");
	  printInstruction("ret", NULL, "", NULL, "");
	  token=scanner.getNextToken();
	  funcDecs();
	}
	else {
	  errorAndQuit(token.getLexeme(), "< ; >");
	}
      }
      else {
	errorAndQuit(token.getLexeme(), "< ; >");
      }
    }
    else {
      errorAndQuit(token.getLexeme(), "< identifier >");
    }
  }
  else if (tokenType == BEGIN_T) {
    return;
  }
  else {
    errorAndQuit(token.getLexeme(), "< function, begin >");
  }
}

void parseClass::funcDecTail() {
  /**
   *	Parses something that can follow a function declaration.
   */
  int tokenType = token.getTypeNum();
  if (tokenType == LPAR_T) {
    token=scanner.getNextToken();
    paramList();
    if (token.getTypeNum() == RPAR_T) {
      token=scanner.getNextToken();
      return;
    }
    else {
      errorAndQuit(token.getLexeme(), "< ) >");
    }
  }
  else if (tokenType == SEMICOLON_T) {
    return;
  }
  else {
    errorAndQuit(token.getLexeme(), "< (, ; >");
  }
}

void parseClass::paramList() {
  /**
   *	Parses a series of parameters in a function declaration.
   */
  int paramType = type();
  if (paramType == INT_T) {
    typeTail(INT_TYPE);
  }
  else if (paramType == BOOLEAN_T) {
    typeTail(BOOLEAN_TYPE);
  }
}

void parseClass::typeTail(int paramType) {
  /**
   *	Parses something that can follow a type in a series of parameters in
   *	a function declaration.
   */
  int tokenType = token.getTypeNum();
  if (tokenType == IDENTIFIER_T) {
    if (!scanner.tableAddParam(token.getLexeme(), VALUE_PARAM, paramType)) {
      duplicateIdentError(token.getLexeme());
    }
    token=scanner.getNextToken();
    identTailParamList();
  }
  else if (tokenType == AMPERSAND_T) {
    token=scanner.getNextToken();
    if (token.getTypeNum() == IDENTIFIER_T) {
      if (!scanner.tableAddParam(token.getLexeme(), REF_PARAM, paramType)) {
	duplicateIdentError(token.getLexeme());
      }
      token=scanner.getNextToken();
      identTailParamList();
    }
    else {
      errorAndQuit(token.getLexeme(), "< identifier >");
    }
  }
  else {
    errorAndQuit(token.getLexeme(), "< identifier, & >");
  }
}

void parseClass::identTailParamList() {
  /**
   *	Parses something that can follow an identifier in a parameter list
   *	in a series of function declarations.
   */
  int tokenType = token.getTypeNum();
  if (tokenType == COMMA_T) {
    token=scanner.getNextToken();
    paramList();
  }
  else if (tokenType == RPAR_T) {
    return;
  }
  else {
    errorAndQuit(token.getLexeme(), "< ,, ) >");
  }
}

void parseClass::closeSourceProgram() {
  /**
   *	Closes the source program by calling the scanner's method.
   */
  scanner.closeSourceProgram();
}

void parseClass::errorAndQuit(string s1, string s2) {
  /**
   *	Error during syntax parsing.
   */
  cerr << "Error: found <" << s1 << "> when expecting one of " << s2 << " at or near line:";
  cerr << endl << "\t";
  scanner.printCurrentLine();
  cin.get();
  exit(1);
}

void parseClass::duplicateIdentError(string lexeme) {
  //Prints a duplicate identifier error.
  cerr << "Duplicate identifier <" << lexeme << ">" << endl;
  scanner.printCurrentLine();
  cin.get();
  exit(1);
}

void parseClass::undecVarError(string lexeme) {
  //Prints an undeclared identifier error.
  cerr << "Undeclared identifier <" << lexeme << ">" << endl;
  scanner.printCurrentLine();
  cin.get();
  exit(1);
}

void parseClass::numParamError() {
  //Prints an error when there is an incorrect number of parameters
  cerr << "Incorrect number of parameters" << endl;
  scanner.printCurrentLine();
  cin.get();
  exit(1);
}