CXX=c++
CFLAGS=-std=c++17
CXXSOURCE=main.cpp cmdline.cpp expr.cpp
OBJS=$(CXXSOURCE:.cpp=.o)
HEADERS=cmdline.h expr.h catch.h

.PHONY: all

all: msdscript

msdscript: $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o msdscript

# Add a rule for building .o files from .cpp source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CFLAGS) -c $< -o $@

.PHONY: run

run: msdscript
	./msdscript

.PHONY: clean

clean:
	rm -f *.o msdscript

.PHONY: printM

printM:
	echo $(CXXSOURCE)
	echo $(HEADERS)
	echo $(CXX)
	echo $(CFLAGS)
