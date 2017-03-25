// Takes in data and creates a queue of QueueParseItem class objects

#ifndef QUEUEPARSELIST_H
#define QUEUEPARSELIST_H

#include <string> // std::string
#include <vector> // std::vector
#include <queue>  // std::queue
#include <mutex>
#include <thread>
#include "QueueParseItem.h"

using namespace std;

class QueueSiteList{
    public:
        // Constructor
        // arguments???????
        QueueParseList();

        // Returns the queue of the file lines
        queue<QueueParseItem> getQueue();

        // Push and pop functions
        void push(QueueParseItem item);
        QueueParseItem pop();

        // get size of queue
        int length();

    private:
        queue<QueueParseItem> queueparselist;
        int len;  // length of queue

        // initialized the mutex lock
        pthread_mutex_t pthreadInitializer();
        pthread_mutex_t lock;

        // conditional variables??
};

#endif
