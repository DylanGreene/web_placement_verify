// Takes vector of sites and creates a queue

#ifndef QUEUESITELIST_H
#define QUEUESITELIST_H

#include <string> // std::string
#include <vector> // std::vector
#include <queue>  // std::queue
#include <mutex>
#include <thread>

using namespace std;

class QueueSiteList{
    public:
        // Constructor that inputs vector of sites and creates a queue
        QueueSiteList(vector<string> sitelist);

        // initializes the queue to sitelist
        void initialize(vector<string> sitelist);

        // Returns the queue of the file lines
        queue<string> getQueue();

        // Push and pop functions
        void push(string s);
        string pop();

        // get size of queue
        int length();

    private:
        queue<string> queuesitelist;
        int len;

        // initialized the mutex lock
        /*void pthreadInitializer();
        pthread_mutex_t lock;*/

        // conditional variables??
};

#endif
