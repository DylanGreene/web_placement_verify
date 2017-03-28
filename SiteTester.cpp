// Site Tester!

#include <iostream>
#include <csignal>
#include <ctime>
#include <pthread.h>
#include <cstdlib>
#include <utility>
#include <functional>
#include <fstream>

#include "ConfigProcessor.h"
#include "Counts.h"
#include "Time.h"
#include "URLFetch.h"
#include "Vectorize.h"
#include "ConcurrentQueue.h"

using namespace std;

// Function declarations
void *fetcher(void *);  // producer
void *parser(void *);   // consumer

// Signal configuration
#define CLOCKID CLOCK_REALTIME
#define SIG SIGUSR1
timer_t timerid;

// Global Variables
ConfigProcessor config;
pthread_t* fetchThreads;
pthread_t* parseThreads;
ConcurrentQueue<pair<string, string> > qParse;
ConcurrentQueue<string> qSites;
Vectorize phrases;
pthread_cond_t empty, filled;
pthread_mutex_t mutexSiteQueue, mutexParseQueue, mutexOutputFile;
int fileNum = 1;
Time timeObject;
string currTime;
ofstream output;
bool loop;

// Catch SIGINT (Ctrl-C)
void signalHandler(int sig){
    cout << "Exiting gracefully" << endl;
    loop = false;
}

void timerHandler(int sig, siginfo_t *si, void *uc){
    // Avoid stray signals
    if (si->si_value.sival_ptr != &timerid) return;

    // Close the old output file if it is open
    if(output){
        output.close();
    }

    // create output file
    fileNum++;
    currTime = timeObject.timeString();
    string filename = to_string(fileNum)+".csv";
    output.open(filename);
    output << "Time, Phrase, Site, Count" << endl;

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
    phrases.setFile(config.get_search_file());
    // Get the URLS to be fetched
    Vectorize urls(config.get_site_file());
    qSites.initialize(urls.getVector());

    // Open the csv output file
    currTime = timeObject.timeString();
    string filename = to_string(fileNum)+".csv";
    output.open(filename);
    output << "Time, Phrase, Site, Count" << endl;

    // Make threads for things
    fetchThreads = (pthread_t*) malloc(sizeof(pthread_t)*config.get_num_fetch());
    parseThreads = (pthread_t*) malloc(sizeof(pthread_t)*config.get_num_parse());

    int ret;
    for (int i = 0; i < config.get_num_fetch(); i++) {
        ret = pthread_create(&fetchThreads[i], NULL, &fetcher, NULL);
        if(ret){
            cerr << "Unable to create fetch thread; return code: " << ret << endl;
       }
    }
    for (int i = 0; i < config.get_num_parse(); i++) {
        ret = pthread_create(&parseThreads[i], NULL, &parser, NULL);
        if(ret){
            cerr << "Unable to create parse thread; return code: " << ret << endl;
       }
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
    its.it_value.tv_sec = config.get_period_fetch();
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;
    timer_settime(timerid, 0, &its, NULL);

    // Run the scheduler
    loop = true;
    while(loop);

    // Exit nicely after done looping

    // Close the old output file if it is open
    if(output){
        output.close();
    }

    // Wait for all the threads
    pthread_cond_broadcast(&empty);
    for(int t = 0; t < config.get_num_fetch(); t++){
        ret = pthread_join(fetchThreads[t], NULL);
        if(ret){
            cerr << "Return code from pthread_join() for fetch thead: " << ret << endl;
        }
    }
    pthread_cond_broadcast(&filled);
    for(int t = 0; t < config.get_num_parse(); t++){
        ret = pthread_join(parseThreads[t], NULL);
        if(ret){
            cerr << "Return code from pthread_join() for fetch thead: " << ret << endl;
        }
    }
    exit(0);
}

// Fetches a URL from the queue and pushes it into the parse queue
void *fetcher(void *args){
    while(loop){
        // Get the URL to be fetched from the queue
        pthread_mutex_lock(&mutexSiteQueue);
        while(qSites.empty()){
            if(!loop) break;
            pthread_cond_wait(&empty, &mutexSiteQueue);
        }
        if(!loop) break;
        auto curr_url = qSites.front();
        qSites.pop();
        pthread_mutex_unlock(&mutexSiteQueue);

        // Fetch the URL
        URLFetch fetched(curr_url);
        string data = fetched.fetch();

        // Put the data into a ParseList Queue
        pthread_mutex_lock(&mutexParseQueue);
        qParse.push(make_pair(curr_url, data));
        pthread_cond_signal(&filled);
        pthread_mutex_unlock(&mutexParseQueue);
    }
    return 0;
}

// Gets string to be parsed from Queue and gets the word counts
void *parser(void *args){
    while(loop){
        pair<string, string> item;
        // Get the string from the parse queue
        pthread_mutex_lock(&mutexParseQueue);
        while(qParse.empty()){
            if(!loop) break;
            pthread_cond_wait(&filled, &mutexParseQueue);
        }
        if(!loop) break;
        item = make_pair<string, string>(qParse.front().first, qParse.front().second);
        qParse.pop();
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutexParseQueue);

        // Get the word counts
        Counts counts;
        counts.createCounts(item.second, phrases.getVector());
        auto c = counts.getCounts();
        for(auto it = c.begin(); it != c.end(); ++it){
            pthread_mutex_lock(&mutexOutputFile);
            output << currTime << ", " << it->first << ", " << item.first << ", " << it->second << endl;
            pthread_mutex_unlock(&mutexOutputFile);
        }
    }
    return 0;
}
