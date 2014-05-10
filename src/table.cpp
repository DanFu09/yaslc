//Dan Fu, December 2012
//This file contains the implementations for the symbol table

//The definitions are found in table.h

#include "headers/table.h"

symbolTableClass::symbolTableClass() {
  //Construction for the symbol table, simply sets the top level to NULL.
  top=NULL;
}

void symbolTableClass::tableAddLevel(string levelName, tableNodeClass *params) {
  //Adds a new scope level to the symbol table.
  scopeLevelClass *newLevel = new scopeLevelClass;
  newLevel->nextOffset=0;
  if (top != NULL) {
    newLevel->nestingLevel=top->nestingLevel + 1;
    newLevel->name = top->name + "." + levelName;
  }
  else {
    newLevel->nestingLevel=0;
    newLevel->name=levelName;
  }
  newLevel->item=NULL;
  newLevel->next=top;
  top=newLevel;
  tableNodeClass* current = params;
  while (current != NULL) {
    tableAddEntry(current->lexeme, current->kind, current->type);
    current=current->next;
  }
}

void symbolTableClass::tableDeleteLevel() {
  //Deletes a scope level from the symbol table.
  while (top->item!=NULL) {
    tableNodeClass *temp = top->item;
    top->item=top->item->next;
    //Delete temp's parameters
    while (temp->params!=NULL) {
      tableNodeClass *temp2 = temp->params;
      temp->params=temp->params->params;
      delete temp2;
    }
    delete temp;
  }
  scopeLevelClass *temp=top;
  top=top->next;
  delete temp;
}

bool symbolTableClass::tableAddEntry(string lexeme, int kind, int type) {
  //Adds an entry to the top scope level of the symbol table.
  tableNodeClass *temp = top->item;
  while (temp!=NULL) {
    if (strcmp(lexeme.c_str(),temp->lexeme.c_str())==0) {
      return false;
    }
    
    temp=temp->next;
  }
  tableNodeClass *newEntry = new tableNodeClass;
  newEntry->lexeme=lexeme;
  newEntry->kind=kind;
  newEntry->type=type;
  newEntry->offset=top->nextOffset;
  newEntry->nestingLevel=top->nestingLevel;
  newEntry->next=top->item;
  newEntry->params=NULL;
  if (kind == VAR_ID || kind == VALUE_PARAM || kind == REF_PARAM) {
    top->nextOffset++;
  }
  top->item=newEntry;
  return true;
}

bool symbolTableClass::tableAddParam(string lexeme, int kind, int type) {
  //Adds a parameter to the last function on the top level of the symbol table.
  tableNodeClass *topNode = top->item;
  if (topNode == NULL || topNode->kind != FUNC_ID) {
    return false;
  }
  tableNodeClass *newEntry = new tableNodeClass;
  tableNodeClass *topParam = topNode->params;
  newEntry->params=NULL;
  newEntry->lexeme=lexeme;
  newEntry->kind=kind;
  newEntry->type=type;
  newEntry->nestingLevel=topNode->nestingLevel;
  newEntry->next=NULL;
  if (topParam == NULL) {
    newEntry->offset=0;
    topNode->params=newEntry;
  }
  else {
    while (topParam->next != NULL) {
      if (strcmp(lexeme.c_str(),topParam->lexeme.c_str())==0) {
	return false;
      }
      topParam=topParam->next;
    }
    newEntry->offset=1 + topParam->offset;
    topParam->next=newEntry;
  }
  return true;
}

tableNodeClass* symbolTableClass::tableLookup(string lex) {
  //Looks up a variable in the symbol table.
  //Stops from the top scope level and moves downwards.
  scopeLevelClass *current=top;
  while (current!=NULL) {
    tableNodeClass* currentItem = current->item;
    tableNodeClass* previousItem = NULL;
    while (currentItem != NULL) {
      if (strcmp(currentItem->lexeme.c_str(),lex.c_str())==0) {
	//Self-organize the list
	if (previousItem!=NULL) {
	  previousItem->next=currentItem->next;
	  currentItem->next=current->item;
	  current->item=currentItem;
	}
	return currentItem;
      }
      previousItem = currentItem;
      currentItem=currentItem->next;
    }
    current=current->next;
  }
  
  return NULL;
}

void symbolTableClass::tableDeleteAllTemps() {
  //Deletes all temporary variables (variables with names of the form "$#")
  //Does not delete special temps (variables with names of the form "$s#")
  tableNodeClass * current = top->item;
  while (current!=NULL) {
    if (current->next != NULL && current->next->lexeme[0]=='$' && current->next->lexeme[1]!='s' &&
      current->next->lexeme[1]!='$') {
      tableNodeClass * temp = current->next;
    current->next = current->next->next;
    while (temp->params != NULL) {
      tableNodeClass *temp2 = temp->params;
      temp->params=temp->params->params;
      delete temp2;
    }
    delete temp;
    top->nextOffset--;
      }
      else {
	current = current->next;
      }
  }
  if (top->item != NULL && top->item->lexeme[0]=='$') {
    top->item = top->item->next;
    top->nextOffset--;
  }
}

void symbolTableClass::tableDeleteSpecialTemps() {
  //Deletes all special temps (variables with names of the form "$s#")
  tableNodeClass * current = top->item;
  while (current!=NULL) {
    if (current->next != NULL && current->next->lexeme[0]=='$' && current->next->lexeme[1]=='s') {
      tableNodeClass * temp = current->next;
      current->next = current->next->next;
      while (temp->params != NULL) {
	tableNodeClass *temp2 = temp->params;
	temp->params=temp->params->params;
	delete temp2;
      }
      delete temp;
      top->nextOffset--;
    }
    else {
      current = current->next;
    }
  }
  if (top->item != NULL && top->item->lexeme[0]=='$') {
    top->item = top->item->next;
    top->nextOffset--;
  }
}

void symbolTableClass::print() {
  //Prints all scope levels of the symbol table.
  scopeLevelClass *current=top;
  cout << "-------------------------" << endl << endl;
  while (current!=NULL) {
    cout << "Name = " << current->name << " Nesting level = " << current->nestingLevel << endl;
    tableNodeClass *currentNode = current->item;
    while (currentNode!=NULL) {
      cout << "Lexeme = " << currentNode->lexeme << ", kind = " << typeToName(currentNode->kind);
      cout << ", type = " << typeToName(currentNode->type) << ", offset = " << currentNode->offset;
      cout << ", nesting level " << currentNode->nestingLevel << endl;
      tableNodeClass *currentParam = currentNode->params;
      if (currentParam != NULL) {
	cout << "\tparam-list:" << endl;
	while (currentParam!=NULL) {
	  cout << "\t\tlexeme = " << currentParam->lexeme << ", kind = " << typeToName(currentParam->kind);
	  cout << ", type = " << typeToName(currentParam->type) << ", offset = " << currentParam->offset;
	  cout << endl;
	  currentParam = currentParam->next;
	}
      }
      currentNode = currentNode->next;
    }
    current=current->next;
    cout << endl;
  }
  cout << endl;
}

string symbolTableClass::typeToName(int type) {
  //Takes a decimal type and converts it to a string.
  switch(type) {
    case FUNC_ID:
      return "FUNC_ID";
    case VAR_ID:
      return "VAR_ID";
    case REF_PARAM:
      return "REF_PARAM";
    case VALUE_PARAM:
      return "VALUE_PARAM";
    case FUNC_ID_TYPE:
      return "FUNC_ID_TYPE";
    case INT_TYPE:
      return "INT_TYPE";
    case BOOLEAN_TYPE:
      return "BOOLEAN_TYPE";
    default:
      return "typeToName: unknown type";
  }
  return "typeToName: unknown type";
}