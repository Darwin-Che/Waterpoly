CXX=g++ 
CXXFLAGS=-std=c++14 -Wall -O -g -MMD -Werror=vla 
# use -MMD to generate dependencies
SOURCES=$(wildcard *.cc)   # list of all .cc files in the current directory
OBJECTS=${SOURCES:.cc=.o}  # .o files depend upon .cc files with same names
DEPENDS=${OBJECTS:.o=.d}   # .d file is list of dependencies for corresponding .cc file
EXEC=watopoly

# First target in the makefile is the default target.
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC) -lX11 -L/usr/X11/lib -I/usr/X11/include
#	strip ${EXEC}

%.o: %.cc 
	$(CXX) -c -o $@ $< $(CXXFLAGS) -lX11 -L/usr/X11/lib -I/usr/X11/include
	
-include ${DEPENDS}

.PHONY: clean
clean:
	rm  -f $(OBJECTS) $(DEPENDS) $(EXEC)
