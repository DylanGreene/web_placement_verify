// Implementation of the ConcurrentQueue Class members

#include "ConcurrentQueue.h"

#include <iostream> // std::cout

// Default constructor
template <typename T>
ConcurrentQueue::ConcurrentQueue() {}

// Initializer that inputs a vector of sites and puts them into a queue
template <typename T>
void ConcurrentQueue::initialize(vector<T> list){
    len = 0;
    for(auto it = sitelist.begin(); it != list.end(); ++it){
        concurrentqueue.push(*it);
        len++;
    }
}

template <typename T>
void ConcurrentQueue::setRestorePoint() {

}

template <typename T>
void ConcurrentQueue::restore() {

}

// Returns the queue
template <typename T>
queue<T> ConcurrentQueue::getQueue() {
    return concurrentqueue;
}

// push and pop functions
template <typename T>
void ConcurrentQueue::push(T s) {
    concurrentqueue.push(s);
    len++;
}

template <typename T>
T ConcurrentQueue::pop() {
    T s;
    s = concurrentqueue.front();
    concurrentqueue.pop();
    len--;
    if (len < 0) {
        len = 0;
    }
    return s;
}

// returns length of queue
template <typename T>
int ConcurrentQueue::length() {
    return len;
}
