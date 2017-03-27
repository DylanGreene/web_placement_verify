// Implementation of the Vectorize Class members
// Takes each line of a file and stores it an a std::vector

#include "Vectorize.h"

#include <fstream>
#include <iostream> // std::cout

// Constructor
Vectorize::Vectorize(string file_name){
    file = file_name;

    // Call the process method to vectorize the file
    process();
}
Vectorize::Vectorize(){
    file = "";
}

// Processes the config file and sets members
void Vectorize::process(){
    // Open the config file as a ifstream
    ifstream ifs(file);

    // Read the file line by line and process each
    string line;
    while(getline(ifs, line)){
        vectorized.push_back(line);
    }
    ifs.close();
}

void Vectorize::setFile(string file_name){
    file = file_name;
    process();
}

// Returns the vectorized STL vector
vector<string> Vectorize::getVector(){
    return vectorized;
}
