CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=-lcurl
SOURCES=.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=site_tester


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
    $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.ccp.o:
    $(CC) $(CFLAGS) $< -o $@

clean:
    rm $(EXECUTABLE) $(OBJECTS)
