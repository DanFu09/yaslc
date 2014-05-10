//Dan Fu, edited September 2012
//Dave A. Berque, Revised August 2011
//This driver program should be used to test the first part of the YASL
//compiler.

//This version tested in Visual Studio .NET 2008

#include "headers/parser.h"
#include <iostream>

int main(int argc, char* argv[])
{   	
  /*symbolTableClass table;
   *	table.tableAddLevel("main", NULL);
   *	table.tableAddEntry("foo", FUNC_ID, FUNC_ID_TYPE);
   *	table.tableAddParam("x", VALUE_PARAM, INT_TYPE);
   *	table.tableAddParam("y", REF_PARAM, BOOLEAN_TYPE);
   *	table.print();
   *	tableNodeClass *function = table.tableLookup("foo");
   *	tableNodeClass *param = function->params;
   *	table.tableAddLevel("foo", param);
   *	table.print();
   *	cin.get();*/
  parseClass parser;
  
  parser.parseProgram();
  
  parser.closeSourceProgram();
  
  cin.get();
  return (0);
}

