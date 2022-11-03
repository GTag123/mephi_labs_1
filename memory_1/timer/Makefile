ifeq ($(OS), Windows_NT)
    USER_OS := Windows
else
    USER_OS := $(shell uname -s)
endif

ifneq ($(USER_OS), Darwin)
	ASAN_OPTIONS := detect_leaks=1
endif

CXXFLAGS = -g -std=c++2a -Wall -Werror -fsanitize=address

all: main.cpp timer.h
	g++ $(CXXFLAGS) main.cpp -o main
	ASAN_OPTIONS=$(ASAN_OPTIONS) ./main

clean:
	rm -f main
