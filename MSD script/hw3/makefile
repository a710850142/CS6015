CXX = g++
CXXFLAGS = -std=c++11
CXXSOURCE = $(wildcard *.cpp)

# Default target
all: msdscript

# Generalized rule that creates object files
# Explicitly include the C++11 standard flag
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Define OBJECTS macro
OBJECTS = $(CXXSOURCE:.cpp=.o)

# Program creates the executable
msdscript: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o msdscript $(OBJECTS)

# Test target
.PHONY: test
test:
	# 添加您的测试命令，例如运行单元测试
	echo "Running tests..."
	# 例如: ./test_msdscript

# Clean deletes all .o and msdscript files
.PHONY: clean
clean:
	rm -f *.o msdscript
