CXXFLAGS=-ansi -pedantic -Wall -std=c++11

analog : main.o Statistiques.o LireLog.o
	g++ $(CXXFLAGS) -o analog Statistiques.o LireLog.o main.o

main.o : main.cpp
	g++ $(CXXFLAGS) -c main.cpp

Statistiques.o : Statistiques.cpp Statistiques.h
	g++ $(CXXFLAGS) -c Statistiques.cpp

LireLog.o : LireLog.cpp LireLog.h
	g++ $(CXXFLAGS) -c LireLog.cpp

clean:
	rm -rf *.o analog
