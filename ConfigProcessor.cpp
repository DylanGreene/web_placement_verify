// Implementation of the ConfigProcessor Class members
// Process the config file for the web placement verifier

#include "ConfigProcessor.h"

#include <string> // std::string
#include <fstream> // std::ifstream
#include <sstream> // std::stringstream
#include <iterator> // std::istream_iterator
#include <vector> // std::vector
#include <iostream> // std::cout, std::endl
#include <sys/stat.h>

// Constructor sets the config file and default values
ConfigProcessor::ConfigProcessor(string file){
    config_file = file;

    // Default values for each param
    period_fetch = 180;
    num_fetch = 1;
    num_parse = 1;
    search_file = "Search.txt";
    site_file = "Sites.txt";
}

// Processes the config file and sets members
void ConfigProcessor::process(){
    // Open the config file as a ifstream
    ifstream ifs(config_file);

    struct stat buffer;
    if (stat (config_file.c_str(), &buffer) != 0) {
        cout << "Config file does not exist!" << endl;
        exit(1);
    }

    // Read the config line by line and process each
    string line;
    while(getline(ifs, line)){
        // Split the config string by the =
        stringstream iss(line);
        vector<string> tokens;
        string token;
        while(getline(iss, token, '=')){
            tokens.push_back(token);
        }

        // Set the proper parameter
        if(tokens[0] == "PERIOD_FETCH"){
            period_fetch = stoi(tokens[1]);
        }else if(tokens[0] == "NUM_FETCH"){
            num_fetch = stoi(tokens[1]);
        }else if(tokens[0] == "NUM_PARSE"){
            num_parse = stoi(tokens[1]);
        }else if(tokens[0] == "SEARCH_FILE"){
            search_file = tokens[1];
        }else if(tokens[0] == "SITE_FILE"){
            site_file = tokens[1];
        }else{ // Not a valid config parameter
            cerr << "Unkown config parameter: " << line << endl;
        }
    }

     // if search file or site file DNE, exit with error message
     if (stat (search_file.c_str(), &buffer) != 0) {
         cout << "Search file " << search_file << " does not exist!" << endl;
         exit(1);
     }

     if (stat (site_file.c_str(), &buffer) != 0) {
         cout << "Site file " << site_file << " does not exist!" << endl;
         exit(1);
     }


    ifs.close();
}

// Displays the conifg settings
void ConfigProcessor::display(){
    cout    << "PERIOD_FETCH: " << period_fetch << "\n"
            << "NUM_FETCH: "    << num_fetch    << "\n"
            << "NUM_PARSE: "    << num_parse    << "\n"
            << "SEARCH_FILE: "  << search_file  << "\n"
            << "SITE_FILE: "    << site_file    << endl;
}

// Setter
void ConfigProcessor::set_config_file(string file){
    config_file = file;
}

// Get functions
int ConfigProcessor::get_period_fetch(){
    return period_fetch;
}
int ConfigProcessor::get_num_fetch(){
    return num_fetch;
}
int ConfigProcessor::get_num_parse(){
    return num_parse;
}
string ConfigProcessor::get_search_file(){
    return search_file;
}
string ConfigProcessor::get_site_file(){
    return site_file;
}
