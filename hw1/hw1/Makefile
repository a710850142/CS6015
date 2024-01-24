all: msdscript

msdscript: main.o cmdline.o
	g++ main.o cmdline.o -o msdscript

main.o: main.cpp
	g++ -c main.cpp

cmdline.o: cmdline.cpp
	g++ -c cmdline.cpp

clean:
	rm -rf *.o msdscript
