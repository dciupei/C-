ifeq "$(shell uname)" "Darwin"
CXX=clang++
else
CXX=g++
endif

ifdef SPEED
CXXFLAGS += -std=c++11 -O3
else
CXXFLAGS += -std=c++11 -Wall -g
endif

ALL=solvesudoku

all: $(ALL)

JUNK=*.o *~ *.dSYM *.gch

clean:
	-rm -rf $(JUNK)

clobber:
	-rm -rf $(JUNK) $(ALL)

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $<

solvesudoku.o: solvesudoku.cpp SudokuGrid.h

solvesudoku: solvesudoku.o
	$(CXX) $(CXXFLAGS) $^ -o $@

