CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=-lcurl -lrt -pthread
OBJECTS=ConfigProcessor.o Counts.o ConcurrentQueue.o Time.o URLFetch.o Vectorize.o SiteTester.o
EXECUTABLE=site-tester

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

ConfigProcessor.o: ConfigProcessor.cpp ConfigProcessor.h
	$(CC) $(CFLAGS) $<

Counts.o: Counts.cpp Counts.h
	$(CC) $(CFLAGS) $<

ConcurrentQueue.o: ConcurrentQueue.cpp ConcurrentQueue.h
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
