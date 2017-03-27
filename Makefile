CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=-lcurl -lrt
SOURCES=SiteTester.cpp ConfigProcessor.cpp Counts.cpp QueueParseItem.cpp QueueParseList.cpp QueueSiteList.cpp Time.cpp URLFetch.cpp Vectorize.cpp
DEPENDS=$(SOURCES)
OBJECTS=ConfigProcessor.o Counts.o QueueParseItem.o QueueParseList.o QueueSiteList.o Time.o URLFetch.o Vectorize.o SiteTester.o
DEPENDS=$(SOURCES)
EXECUTABLE=site_tester

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

ConfigProcessor.o: ConfigProcessor.cpp ConfigProcessor.h
	$(CC) $(CFLAGS) $<

Counts.o: Counts.cpp Counts.h
	$(CC) $(CFLAGS) $<

QueueParseItem.o: QueueParseItem.cpp QueueParseItem.h
	$(CC) $(CFLAGS) $<

QueueParseList.o: QueueParseList.cpp QueueParseList.h
	$(CC) $(CFLAGS) $<

QueueSiteList.o: QueueSiteList.cpp QueueSiteList.h
	$(CC) $(CFLAGS) $<

Time.o: Time.cpp Time.h
	$(CC) $(CFLAGS) $<

URLFetch.o: URLFetch.cpp URLFetch.h
	$(CC) $(CFLAGS) $<

Vectorize.o: Vectorize.cpp Vectorize.h
	$(CC) $(CFLAGS) $<

SiteTester.o: SiteTester.cpp
	$(CC) $(CFLAGS) $<

clean:
	rm $(EXECUTABLE) $(OBJECTS)
