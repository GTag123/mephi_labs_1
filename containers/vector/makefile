all: main.o
	g++ --std=c++2a main.o -o main
	./main

main.o: main.cpp
	g++ --std=c++2a -c main.cpp -o main.o

clean:
	rm -f main.o array.o main
