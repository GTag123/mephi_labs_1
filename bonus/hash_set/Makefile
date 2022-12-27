SOURCES := $(wildcard *.cpp)
RESULT := main
SCRIPTS := $(wildcard *.sh)
SCRIPT_TARGETS := $(SCRIPTS:.sh=_run)

OBJECTS := $(SOURCES:.cpp=.o)
CFLAGS := -std=c++14 -O2 -Wall

all: run

run: compile input.txt
	./$(RESULT) < input.txt

%_run: %.sh
	/bin/bash $<

compile: $(SOURCES) $(RESULT) $(SCRIPT_TARGETS)

.cpp.o: $(wildcard *.h)
	g++ -c $(CFLAGS) $< -o $@

$(RESULT): $(OBJECTS)
	g++ main.o -o $(RESULT)

clean:
	rm -f $(OBJECTS) $(RESULT)

