// Implementation of the ConcurrentQueue Class members

#include "ConcurrentQueue.h"

template <class T>
bool ConcurrentQueue<T>::empty() const{  // Return truee if empty
    return concurrentqueue.empty();
}

template <class T>
void ConcurrentQueue<T>::push(T const& elem){
    concurrentqueue.push(elem);
}

template <class T>
void ConcurrentQueue<T>::pop(){
    if(concurrentqueue.empty()){
        throw out_of_range("Stack<>::pop(): empty queue");
    }
    // Remove element
    concurrentqueue.pop();
}

template <class T>
T ConcurrentQueue<T>::front() const {
    if(concurrentqueue.empty()){
        throw out_of_range("Stack<>::top(): empty queue");
    }
    return concurrentqueue.front();
}

// Initializer that inputs a vector of sites and puts them into a queue
template <class T>
void ConcurrentQueue<T>::initialize(vector<T> list){
    for(auto it = list.begin(); it != list.end(); ++it) {
        concurrentqueue.push(*it);
    }
}

// Allow for explicit instantiation
template class ConcurrentQueue<string>;
template class ConcurrentQueue<pair<string, string> >;
