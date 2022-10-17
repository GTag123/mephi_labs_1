CXXFLAGS = -g -std=c++2a -Wall -Werror -fsanitize=address

all: main.cpp array.h
	g++ $(CXXFLAGS) main.cpp -o main
	ASAN_OPTIONS=detect_leaks=1 ./main

clean:
	rm -f main
