SOURCES := main.cpp text_editor.cpp task.cpp logged_command_wrapper.cpp
OBJECTS := $(SOURCES:.cpp=.o)
CFLAGS := -std=c++2a -Wall -Werror
RESULT := main

all: run

run: compile
	./$(RESULT)

compile: $(SOURCES) $(RESULT)

.cpp.o: $(wildcard *.h)
	g++ -c $(CFLAGS) $< -o $@

$(RESULT): $(OBJECTS)
	g++ $(OBJECTS) $(LDFLAGS) -o $(RESULT)

clean:
	rm -f $(OBJECTS) $(RESULT)
