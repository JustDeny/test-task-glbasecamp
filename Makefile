all: finder.lib 
	g++ -std=c++17 main.cpp Finder.h finder.lib
finder.lib: Finder.o
	ar rcs finder.lib Finder.o
Finder.o:
	g++ -std=c++17 -c Finder.cpp -o Finder.o

clean:
	-rm -f finder.lib Finder.o
	del finder.lib Finder.o
	

