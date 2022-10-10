all: main.o
	g++ --std=c++2a -fsanitize=address main.o -o main
	ASAN_OPTIONS=detect_leaks=1 ./main

main.o: main.cpp array.h
	g++ --std=c++2a -fsanitize=address -c main.cpp -o main.o

clean:
	rm -f main.o main
