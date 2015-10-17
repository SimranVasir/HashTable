Generate: Generate.o
	g++ -Wall -g -o Generate Generate.o

Generate.o: Generate.cpp
	g++ -Wall -g -c Generate.cpp 
