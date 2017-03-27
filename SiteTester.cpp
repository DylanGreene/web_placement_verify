// Site Tester!

#include <iostream>
#include <csignal>
#include <ctime>
#include <pthread.h>

#include "ConfigProcessor.h"
#include "Counts.h"
#include "QueueParseItem.h"
#include "ConcurrentQueue.h"
#include "Time.h"
#include "URLFetch.h"
#include "Vectorize.h"

using namespace std;

// Function declarations
void* fetcher();  // producer
void* parser();   // consumer
void* initializeThread();

// Signal configuration
#define CLOCKID CLOCK_REALTIME
#define SIG SIGUSR1
timer_t timerid;

// Global Variables
ConfigProcessor config;
pthread_t* fetchThreads;
pthread_t* parseThreads;
ConcurrentQueue<QueueParseItem> qParse;
ConcurrentQueue<string> qSites;
cond_t empty, fill;
mutex_t mutexSiteQueue, mutexParseQueue;

// Catch SIGINT (Ctrl-C)
void signalHandler(int sig){
    cout << "Exiting gracefully" << endl;
    exit(0);
}

void timerHandler(int sig, siginfo_t *si, void *uc){
    // Avoid stray signals
    if (si->si_value.sival_ptr != &timerid) return;

    Time tm;
    cout << tm.timeString() << endl;

    // Get the URLS to be fetched
    Vectorize urls(config.get_site_file());
    qSites.initialize(urls.getVector());

    // Broadcast threads to wake up
    pthread_cond_broadcast(&empty);
}

// Main Execution
int main(int argc, char *argv[]){
    // Set up to catch SIGINT, SIGHUP
    signal(SIGINT, signalHandler);
    signal(SIGHUP, signalHandler);

    // Parse the config file
    string configfile = "Config.txt";
    if(argc == 2){
        configfile = argv[1];
    }
    config.set_config_file(configfile);
    config.process();

    // Get the search phrases into a vector
    Vectorize phrases(config.get_search_file());
    // Get the URLS to be fetched
    Vectorize urls(config.get_site_file());
    qSites.initialize(urls.getVector());

    // Make threads for things
    fetchThreads = (pthread_t*) malloc(sizeof(pthread_t)*config.get_num_fetch());
    parseThreads = (pthread_t*) malloc(sizeof(pthread_t)*config.get_num_parse());

    for (int i = 0; i < config.get_num_fetch(); i++) {
        pthread_create(&fetchThreads[i], NULL, fetcher, NULL);
    }
    for (int i = 0; i < config.get_num_parse(); i++) {
        pthread_create(&parseThreads[i], NULL, parser, NULL);
    }

    // Sructs for the timer event scheduler
    struct sigevent sev;
    struct itimerspec its;
    struct sigaction sa;

    // Establish the handler for timer signal
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = timerHandler;
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

// Fetches a URL from the queue and pushes it into the parse queue
void* fetcher() {
    // Get the URL to be fetched from the queue
    pthread_mutex_lock(&mutexSiteQueue);
    while (qSites.length() == 0) {
        pthread_cond_wait(&empty, &mutexSiteQueue)
    }
    string curr_url = qSites.pop();
    pthread_cond_signal(&fill);
    pthread_mutex_unlock(&mutexSiteQueue)

    // Fetch the URL
    URLFetch fetched(curr_url);
    string data = fetched.fetch();

    // Create queue item to push into qParse
    QueueParseItem item(curr_url, data);

    // Put the data into a ParseList Queue
    pthread_mutex_lock(&mutexParseQueue);
    qParse.push(item);
    pthread_mutex_unlock(&mutexParseQueue);
}

// Gets string to be parsed from Queue and gets the word counts
void* parser() {
    // Get the string from the parse queue
    pthread_mutex_lock(&mutexParseQueue);
    while (qParseList.length() == 0) {
        pthread_cond_wait(&fill, &mutex)
    }
    QueueParseItem item = qParse.pop();
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&mutexParseQueue);

    // Get the word counts
    Counts counts;
    counts.createCounts(item.getData(), phrases.getVector());

    cout << item.getSite() << endl;
    auto c = counts.getCounts();
    for(auto it = c.begin(); it != c.end(); ++it){
        cout << "\t" << it->first << " " << it->second << endl;
    }
}
