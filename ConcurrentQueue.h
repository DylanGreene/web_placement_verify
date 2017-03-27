// Takes vector of sites and creates a queue

#ifndef CONCURRENTQUEUE_H
#define CONCURRENTQUEUE_H

#include <iostream> // std::cout
#include <string> // std::string
#include <vector> // std::vector
#include <queue>  // std::queue
#include <cstdlib>
#include <utility>
#include <stdexcept>

using namespace std;

template <class T>
class ConcurrentQueue {
private:
    queue<T> concurrentqueue;

public:
    // initializes the queue to inputted vector
    void initialize(vector<T> list);

    // Push and pop functions
    void push(T const&); // Push Element
    void pop(); // Pop element
    T front() const; // Return the front element
    bool empty() const;  // Return truee if empty
};

#endif
