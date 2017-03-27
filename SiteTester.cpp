// Site Tester!

#include <iostream>
#include <csignal>
#include <ctime>
#include <pthread.h>

#include "ConfigProcessor.h"
#include "Counts.h"
#include "QueueParseItem.h"
#include "QueueParseList.h"
#include "QueueSiteList.h"
#include "Time.h"
#include "URLFetch.h"
#include "Vectorize.h"

using namespace std;

// Function declarations
void* fetcher();  // producer
void* parser();   // consumer
void* initializeThread();
cond_t empty, fill;
mutex_t mutex;

// Signal configuration
#define CLOCKID CLOCK_REALTIME
#define SIG SIGUSR1
timer_t timerid;

// Global Variables
ConfigProcessor config;
pthread_t* fetchThreads;
pthread_t* parseThreads;


// Catch SIGINT (Ctrl-C)
void SIGINTHandler(int sig){
    cout << "Exiting gracefully" << endl;
    exit(0);
}

void signalHandler(int sig, siginfo_t *si, void *uc){
    // Avoid stray signals
    if (si->si_value.sival_ptr != &timerid) return;

    Time tm;
    cout << tm.timeString() << endl;

    // Get the search phrases into a vector
    Vectorize phrases(config.get_search_file());

    // Get the URLS to be fetched
    Vectorize urls(config.get_site_file());
    QueueSiteList qSiteList(urls.getVector());
    QueueParseList pParseList;

    // Get data for each URL
    while(qSiteList.length() > 0){
        // critical section (while loop on cond not ready)


        }

        URLFetch fetched(curr_url);
        string data = fetched.fetch();

        // create queue item to push into pParseList
        QueueParseItem item(curr_url, data);

        // Put the data into a ParseList Queue
        pParseList.push(item);
    }

    // Get counts of each search word
    while(pParseList.length() > 0){

        Counts counts;
        counts.createCounts(item.getData(), phrases.getVector());
        cout << item.getSite() << endl;

        auto c = counts.getCounts();
        for(auto it = c.begin(); it != c.end(); ++it){
            cout << "\t" << it->first << " " << it->second << endl;
        }
    }
}

// Main Execution
int main(int argc, char *argv[]){
    // Set up to catch SIGINT
    signal(SIGINT, SIGINTHandler);

    // Parse the config file
    string configfile = "Config.txt";
    if(argc == 2){
        configfile = argv[1];
    }
    config.set_config_file(configfile);
    config.process();

    // Make threads for things
    fetchThreads = (pthread_t*) malloc(sizeof(pthread_t)*config.get_num_fetch());
    parseThreads = (pthread_t*) malloc(sizeof(pthread_t)*config.get_num_parse());

    for (int i = 0; i < config.get_num_fetch(); i++) {
        pthread_create(&fetchThreads[i], NULL, initializeThread, NULL);
    }
    for (int i = 0; i < config.get_num_parse(); i++) {
        pthread_create(&parseThreads[i], NULL, initializeThread, NULL);
    }

    // Sructs for the timer event scheduler
    struct sigevent sev;
    struct itimerspec its;
    struct sigaction sa;

    // Establish the handler for timer signal
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = signalHandler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIG, &sa, NULL);

    // Create timer signal
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIG;
    sev.sigev_value.sival_ptr = &timerid;
    timer_create(CLOCKID, &sev, &timerid);

    // Specify timer settings
    its.it_value.tv_sec = 5;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;
    timer_settime(timerid, 0, &its, NULL);

    // Run the scheduler
    while (1);
}

void* initializeThread() {
    // keep thread waiting
    pthread_cond_wait();
}

void put(void* q, void* item) {
    q.push(item);
}

void* get(void* q) {
    return q.pop();
}

void* fetcher(QueueSiteList qSiteList) {
    pthread_mutex_lock(&mutex);
    while (qSiteList.length() == 0) {
        pthread_cond_wait(&empty, &mutex)
    }
    string curr_url = qSiteList.pop();

    pthread_cond_signal(&fill);
    pthread_mutex_unlock(&mutex);

    return curr_url;
}

void* parser(QueueParseList qParseList) {
    pthread_mutex_lock(&mutex);
    while (qParseList.length() == 0) {
        pthread_cond_wait(&full, &mutex)
    }
    QueueParseItem item = pParseList.pop();
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&mutex);

    return item;
}
