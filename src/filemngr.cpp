//Dan Fu, edited September 9, 2012
//Dave A. Berque, Revised August 2011
//This file contains method implementations for the
//fileManagerClass

//The class header is found in filemngr.h

#include "headers/filemngr.h"


fileManagerClass::fileManagerClass()
//Precondition: None.
//Postcondition: The constructor has opened the file which is to be
//               processed and has inialized the current line number to 0
{  
  //Initialiaze private variables
  autoPrintStatus=false;
  
  char filename[MAXCELLSPERSTRING];
  
  cout << "Enter file name to compile: ";
  cin.getline (filename, MAXCELLSPERSTRING);
  fileToCompile.open(filename);
  
  if (fileToCompile.fail())
  {  cout << "Error, the file: " << filename << " was not opened." << endl;
    cout << "Press return to terminate program." << endl;
    cin.get();
    exit(1);
  }  
  currentLineNumber = 0;
}

int fileManagerClass::getNextChar()
//Precondition:  The source file associated with the owning object has
//               been prepared for reading.
//Postcondition: The next character from the input file has been read and
//               returned by the function.  If another chacter could not
//               be read because END OF FILE was reached then EOF is
//               returned.
{  if ((currentLineNumber == 0) ||
  (charToReadNext == strlen(currentLine)))
{  if (fileToCompile.peek() == EOF) 
  return (EOF);
  fileToCompile.getline(currentLine, MAXCELLSPERSTRING);
  strcat(currentLine, "\n");
  currentLineNumber++;
  charToReadNext = 0;
  //If line printing is turned on, print the line that was just read
  if (autoPrintStatus) {
    printCurrentLine();
  }
}
return(currentLine[charToReadNext++]);
}


void fileManagerClass::closeSourceProgram()
//Precondition:  The file belonging to the object owning this routine has
//               been opened.
//Postcondition: The file belonging to the object owning this routine has
//               been closed.
{  fileToCompile.close();
  //Print the number of lines processed
  cout << "YASLC-DF has just compiled " << numLinesProcessed() << " lines of code." << endl;
}

void fileManagerClass::pushBack()
//When this function is called, the index into the buffer
//charToReadNext should be decremented by one. This has the effect of returning the last character to the
//buffer. If the value of charToReadNext is already zero when this function is called no changes should
//take place (i.e. do not decrement the value.)
{
  if (charToReadNext > 0) 
    charToReadNext--;
}

void fileManagerClass::printCurrentLine()
//Add a void member function to the fileManagerClass named printCurrentLine() that takes no
//parameters. When this function is called the current line number followed by a hyphen a space and the
//contents of the line (i.e. the contents of the buffer) should be printed.
{
  if (currentLineNumber >=1) {
    cout << currentLineNumber << "- " << currentLine;
  }
}

void fileManagerClass::setPrintStatus(bool newStatus)
//Add a void member function named setPrintStatus(newStatus) where newStatus
//is a bool parameter. This function should change the value of the autoPrintStatus variable to match
//newStatus.
{
  autoPrintStatus=newStatus;
}

int fileManagerClass::numLinesProcessed()
//Add an int member function named numLinesProcessed() that takes no parameters and returns the
//cumulative number of lines that have been processed (read into the buffer) so far.
{
  return currentLineNumber;
}