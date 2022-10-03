all: main.o array.o
	g++ --std=c++2a main.o array.o -o main
	./main

array.o: array.cpp
	g++ --std=c++2a -c array.cpp -o array.o

main.o: main.cpp
	g++ --std=c++2a -c main.cpp -o main.o

clean:
	rm -f main.o array.o main
