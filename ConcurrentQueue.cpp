// Implementation of the ConcurrentQueue Class members

#include "ConcurrentQueue.h"

#include <iostream> // std::cout

// Default constructor
template <typename T>
ConcurrentQueue<T>::ConcurrentQueue(){
    len = 0;
}

// Initializer that inputs a vector of sites and puts them into a queue
template <typename T>
void ConcurrentQueue<T>::initialize(vector<T> list){
    len = 0;
    for(auto it = list.begin(); it != list.end(); ++it){
        concurrentqueue.push(*it);
        len++;
    }
}

// Returns the queue
template <typename T>
queue<T> ConcurrentQueue<T>::getQueue() {
    return concurrentqueue;
}

// push and pop functions
template <typename T>
void ConcurrentQueue<T>::push(T s) {
    concurrentqueue.push(s);
    len++;
}

template <typename T>
T ConcurrentQueue<T>::pop() {
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
int ConcurrentQueue<T>::length() {
    return len;
}
