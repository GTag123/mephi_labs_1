all: main.o tasks.o
	g++ --std=c++2a main.o tasks.o -o main

tasks.o: tasks.cpp
	g++ --std=c++2a -c tasks.cpp -o tasks.o

main.o:
	g++ --std=c++2a -c main.cpp -o main.o

clean:
	rm -f main.o tasks.o main
