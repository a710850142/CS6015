# Corrected variable names and added the correct command for linking the object files into an executable
CXXSOURCE = expr.cpp cmdline.cpp main.cpp
OBJSOURCE = expr.o cmdline.o main.o

# The target for making the final executable
msdscript: $(OBJSOURCE)
	clang++ -o msdscript $(OBJSOURCE)

# Compilation rules for the object files
main.o: main.cpp
	clang++ -c -std=c++17 main.cpp

cmdline.o: cmdline.h cmdline.cpp
	clang++ -c -std=c++17 cmdline.cpp

expr.o: expr.h expr.cpp
	clang++ -c -std=c++17 expr.cpp

# Target for running tests
test:
	./msdscript --test

# Target for cleaning up the build artifacts
clean:
	rm -f *.o msdscript
