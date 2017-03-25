CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=-lcurl -lrt
SOURCES=SiteTester.cpp ConfigProcessor.cpp Counts.cpp QueueParseItem.cpp QueueParseList.cpp QueueSiteList.cpp Time.cpp URLFetch.cpp Vectorize.cpp
DEPENDS=$(SOURCES)
OBJECTS=SiteTester.o ConfigProcessor.o Counts.o QueueParseItem.o QueueParseList.o QueueSiteList.o Time.o URLFetch.o Vectorize.o
DEPENDS=$(SOURCES)
EXECUTABLE=site_tester

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
    $(CC) $(LDFLAGS) $^ -o $@

$(OBJECTS): $(DEPENDS)
    $(CC) $(CFLAGS) $< -o $@

clean:
    rm $(EXECUTABLE) $(OBJECTS)
