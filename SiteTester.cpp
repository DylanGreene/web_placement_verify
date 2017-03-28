// Site Tester!

#include <iostream>
#include <csignal>
#include <ctime>
#include <pthread.h>
#include <cstdlib>
#include <utility>
#include <functional>

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
pthread_mutex_t mutexSiteQueue, mutexParseQueue;

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
    cout << "broadcasting" << endl;
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

    // Make threads for things
    fetchThreads = (pthread_t*) malloc(sizeof(pthread_t)*config.get_num_fetch());
    parseThreads = (pthread_t*) malloc(sizeof(pthread_t)*config.get_num_parse());

    cout << "making threads" << endl;
    for (int i = 0; i < config.get_num_fetch(); i++) {
        pthread_create(&fetchThreads[i], NULL, &fetcher, NULL);
    }
    for (int i = 0; i < config.get_num_parse(); i++) {
        pthread_create(&parseThreads[i], NULL, &parser, NULL);
    }
    cout << "threads made" << endl;

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
void *fetcher(void *args){
  cout << "in fetcher" << endl;
  while (!qSites.empty()) {
    // Get the URL to be fetched from the queue
    pthread_mutex_lock(&mutexSiteQueue);
    while(qSites.empty()){
        pthread_cond_wait(&empty, &mutexSiteQueue);
    }
    auto curr_url = qSites.front();
    qSites.pop();
    cout << "popping in fetcher" << endl;

    // Fetch the URL
    URLFetch fetched(curr_url);
    string data = fetched.fetch();
    cout << "current url being fetched... " << curr_url << endl;
    pthread_mutex_unlock(&mutexSiteQueue);

    // Put the data into a ParseList Queue
    pthread_mutex_lock(&mutexParseQueue);
    qParse.push(make_pair(curr_url, data));
    cout << "pushed" << endl;
    pthread_cond_signal(&filled);
    cout << "filled signaled" << endl;
    pthread_mutex_unlock(&mutexParseQueue);
  }
  return 0;
}

// Gets string to be parsed from Queue and gets the word counts
void *parser(void *args){
  cout << "in parser" << endl;
  pair<string, string> item;
  // Get the string from the parse queue
  pthread_mutex_lock(&mutexParseQueue);
  while(qParse.empty()){
      pthread_cond_wait(&filled, &mutexParseQueue);
  }
  item = make_pair<string, string>(qParse.front().first, qParse.front().second);
  qParse.pop();
  cout << "popping in parser" << endl;
  pthread_cond_signal(&empty);
  pthread_mutex_unlock(&mutexParseQueue);

  // Get the word counts
  Counts counts;
  counts.createCounts(item.second, phrases.getVector());
  cout << "creating counts" << endl;
  //cout << item.first << endl;
  auto c = counts.getCounts();
  for(auto it = c.begin(); it != c.end(); ++it){
      cout << it->first << " " << it->second << endl;
  }
  return 0;
}
