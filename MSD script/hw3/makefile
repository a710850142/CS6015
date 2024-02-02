# Macros
CXX = g++
CXXFLAGS = -std=c++11
CXXSOURCE = ${wildcard *.cpp}

# Default target
all: msdscript

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

OBJECTS = $(CXXSOURCE:.cpp=.o)


msdscript: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o msdscript $(OBJECTS)


.PHONY: clean
clean:
	rm -f *.o msdscript
