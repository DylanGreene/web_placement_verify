// Process the config file for the web placement verifier

#ifndef CONFIG_PROCESSOR_H
#define CONFIG_PROCESSOR_H

#include <string> // std::string

using namespace std;

class Config_Processor{
public:
    // Constructor sets the config file
    Config_Processor(string file="Config.txt");

    // Displays the settings from the config file
    void display();

    // Getters
    int get_period_fetch();
    int get_num_fetch();
    int get_num_parse();
    string get_search_file();
    string get_site_file();

private:
    // Config File
    string config_file;

    // Configuration Parameters
    int period_fetch; // Time in seconds between fetches
    int num_fetch; // Number of fetch threads
    int num_parse; // Number of parsing threads
    string search_file; // File containing the search strings
    string site_file; // File containing the sites to query

    // Performs the processing of the config file
    void process();
};

#endif
