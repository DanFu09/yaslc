VPATH=src:src/headers

CC=g++
CFLAGS=-c
LDFLAGS=
SOURCES=filemngr.cpp table.cpp scanner.cpp pstack.cpp parser.cpp yaslc.cpp
HEADERS=general.h filemngr.h table.h scanner.h pstack.h parser.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=yaslc

all: $(HEADERS) $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	rm $(OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE)