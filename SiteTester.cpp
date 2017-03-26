// Site Tester!

#include <iostream>
#include <csignal>
#include <ctime>

#include "ConfigProcessor.h"
#include "Counts.h"
#include "QueueParseItem.h"
#include "QueueParseList.h"
#include "QueueSiteList.h"
#include "Time.h"
#include "URLFetch.h"
#include "Vectorize.h"

using namespace std;

// Signal configuration
#define CLOCKID CLOCK_REALTIME
#define SIG SIGUSR1
timer_t timerid;

// Global Variables
ConfigProcessor config;

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
        string curr_url = qSiteList.pop();
        URLFetch fetched(curr_url);
        string data = fetched.fetch();

        // create queue item to push into pParseList
        QueueParseItem item(curr_url, data);

        // Put the data into a ParseList Queue
        pParseList.push(item);
    }

    // Get counts of each search word
    while(pParseList.length() > 0){
        QueueParseItem item = pParseList.pop();

        Counts counts;
        counts.createCounts(item.getData(), phrases.getVector());
        cout << item.getSite() << endl;
        for(auto it = counts.getCounts().begin(); it != counts.getCounts().end(); ++it){
            cout << "\t" << it->first << " " << it->second << endl;
        }

    }
}

// Main Execution
int main(int argc, char *argv[]){
    // Parse the config file
    string configfile = "Config.txt";
    if(argc == 2){
        configfile = argv[1];
    }
    config.set_config_file(configfile);
    config.process();

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
    its.it_value.tv_sec = 15;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;
    timer_settime(timerid, 0, &its, NULL);

    // Run the scheduler
    while (1);
}
