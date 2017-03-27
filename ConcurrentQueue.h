// Takes vector of sites and creates a queue

#ifndef CONCURRENTQUEUE_H
#define CONCURRENTQUEUE_H

#include <string> // std::string
#include <vector> // std::vector
#include <queue>  // std::queue
#include <mutex>
#include <thread>

using namespace std;

template <typename T>
class ConcurrentQueue{
    public:
        // default constructor
        ConcurrentQueue();

        // initializes the queue to inputted vector
        void ConcurrentQueue::initialize(vector<T> list);

        // Returns the queue of the file lines
        queue<T> getQueue();

        // Push and pop functions
        void push(T s);
        string pop();

        // get size of queue
        int length();

    private:
        queue<T> concurrentqueue;
        int len;

};

#endif
