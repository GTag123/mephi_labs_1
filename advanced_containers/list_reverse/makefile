SOURCES := main.cpp task.cpp
OBJECTS := $(SOURCES:.cpp=.o)
CFLAGS := -std=c++2a -Wall -Werror
RESULT := main

all: run

run: compile
	./$(RESULT)

compile: $(SOURCES) check $(RESULT)

check:
	./check.sh

.cpp.o:
	g++ -c $(CFLAGS) $< -o $@

$(RESULT): $(OBJECTS)
	g++ $(OBJECTS) $(LDFLAGS) -o $(RESULT)

clean:
	rm -f $(OBJECTS) $(RESULT)
