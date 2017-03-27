// Implementation of the QueueSiteList Class members
// Takes vector of sites and creates a queue

#include "QueueSiteList.h"

#include <iostream> // std::cout

// Constructor that inputs a vector of sites and puts them into a queue
QueueSiteList::QueueSiteList(vector<string> sitelist){
    len = 0;
    for(auto it = sitelist.begin(); it != sitelist.end(); ++it){
        queuesitelist.push(*it);
        len++;
    }
}

void QueueSiteList::initialize(vector<string> sitelist) {
  len = 0;
  for(auto it = sitelist.begin(); it != sitelist.end(); ++it){
      queuesitelist.push(*it);
      len++;
  }
}
// Initialized the mutex lock
/*void QueueSiteList::pthreadInitializer() {
    lock = PTHREAD_MUTEX_INITIALIZER;
}*/

// Returns the vectorized STL vector
queue<string> QueueSiteList::getQueue() {
    return queuesitelist;
}

// push and pop functions
void QueueSiteList::push(string s) {
    queuesitelist.push(s);
    len++;
}

string QueueSiteList::pop() {
    string s;
    s = queuesitelist.front();
    queuesitelist.pop();
    len--;
    if (len < 0) {
        len = 0;
    }
    return s;
}

// returns length of queue
int QueueSiteList::length() {
    return len;
}
