//Dan Fu, edited October 2012
//This file contains the implementations for the
//pstack class and the stackCell class

//The headers are found in pstack.h

#include "headers/pstack.h"

pStackClass::pStackClass() {
  /**
   C *onstructor for the pStackClass
   */
  top=NULL;
}

void pStackClass::push(tokenClass theToken) {
  /**
   P *ushes theToken onto the stack
   */
  stackCell *tempCell = new stackCell;
  tempCell->next = top;
  top = tempCell;
  tempCell->type = theToken.getTypeNum();
  tempCell->subType = theToken.getSubtypeNum();
  tempCell->symTableVariable = theToken.symTableVariable;
}

tokenClass pStackClass::pop() {
  /**
   R *eturns the topmost token and removes it from the stack.
   If the stack is empty, prints and error message and terminates.
   */
  if (top == NULL) {
    cerr << "Error: Empty stack popped." << endl;
    cin.get();
    exit(1);
  }
  tokenClass toReturn (top->type, top->subType, "");
  toReturn.symTableVariable=top->symTableVariable;
  stackCell *tempCell=top;
  top=top->next;
  delete tempCell;
  return toReturn;
}

bool pStackClass::isThereATerminalOnTop() {
  /**
   R *eturns true if there is a terminal on the top
   of the stack, false otherwise
   */
  return !(top == NULL || top->type == E_T);
}

tokenClass pStackClass::returnTopMostTerminal() {
  /**
   R *eturns the topmost terminal on the stack. Prints an error
   and terminates of there is no terminal in the stack.
   */
  if (top == NULL) {
    cerr << "Error: No terminal on empty stack." << endl;
    cin.get();
    exit(1);
  }
  stackCell *tempCell=top;
  while (tempCell!=NULL && tempCell->type==E_T) {
    tempCell=tempCell->next;
  }
  if (tempCell==NULL||tempCell->type==E_T) {
    cerr << "Error: No terminal on top." << endl;
  }
  tokenClass toReturn (tempCell->type, tempCell ->subType, "");
  toReturn.symTableVariable=tempCell->symTableVariable;
  return toReturn;
}

void pStackClass::print() {
  /**
   P *rints all the elements of the stack.
   */
  stackCell *tempCell=top;
  while (tempCell!=NULL) {
    tokenClass token (tempCell->type, tempCell->subType, "");
    cout << token.getType() << "\t" << token.getSubtype() << endl;
    tempCell=tempCell->next;
  }
}