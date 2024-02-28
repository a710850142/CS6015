CXX = g++
CXXFLAGS = -std=c++11
CXXSOURCE = $(wildcard *.cpp)

# Default target only builds msdscript
all: msdscript

# Generalized rule that creates object files
# Explicitly include the C++11 standard flag
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Define OBJECTS macro for msdscript, excluding test_msdscript and exec
OBJECTS = $(filter-out test_msdscript.o exec.o, $(CXXSOURCE:.cpp=.o))

# Program creates the executable for msdscript
msdscript: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o msdscript $(OBJECTS)

# Test target for msdscript
.PHONY: test
test: msdscript
	./msdscript --test

# Clean deletes all .o, msdscript, and test_msdscript files
.PHONY: clean
clean:
	rm -f *.o msdscript test_msdscript

# Documentation
doc: msdscript
	cd doc && doxygen

# Additional sources and objects for test_msdscript
TEST_SOURCES = test_msdscript.cpp exec.cpp
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)

# Target to compile and link test_msdscript, separate from the default build
test_msdscript: $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o test_msdscript $(TEST_OBJECTS)

# Specialized rule for compiling test_msdscript and exec object files
# No header dependency specified as it might not be needed or exist
test_msdscript.o: test_msdscript.cpp
	$(CXX) $(CXXFLAGS) -c test_msdscript.cpp -o test_msdscript.o

exec.o: exec.cpp
	$(CXX) $(CXXFLAGS) -c exec.cpp -o exec.o
