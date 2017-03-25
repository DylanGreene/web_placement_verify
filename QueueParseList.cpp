// Implementation of the QueueParseList Class members

#include "QueueParseList.h"
#include "QueueParseItem.h"

#include <iostream> // std::cout

// Constructor
QueueParseList::QueueParseList(vector<string> parselist){
    //TODO: what are the arguments for constructor?? what does the curl return?
    for (i = 0; i < sitelist.length(); i++) {
        queueparselist.push(parselist[i])
    }

    // initialize length of queue to 0
    len = 0;
}

// Initialize mutex lock
void QueueSiteList::pthreadInitializer() {
    lock = PTHREAD_MUTEX_INITIALIZER;
}

// Returns the queue
queue<QueueParseItem> QueueParseList::getQueue(){
    return queueparselist;
}

// Push function
void QueueParseList::push(QueueParseItem item) {
    queueparselist.push(item);
    len++;
}

// Pop function
QueueParseItem QueueParseList::pop() {
    QueueParseItem q;
    q = queueparselist.front();
    queueparselist.pop();
    len--;
    if (len < 0) {
        len = 0;
    }
    return q;
}

// returns the length of the queue
int QueueParseList::length() {
    return len;
}
