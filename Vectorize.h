// Takes each line of a file and stores it an a std::vector

#ifndef VECTORIZE_H
#define VECTORIZE_H

#include <string> // std::string
#include <vector> // std::vector

using namespace std;

class Vectorize{
public:
    // Constructor sets the file to vectorize
    Vectorize(string file_name);

    // Returns the vector of the file lines
    vector<string> getVector();

private:
    // The file to be processed and vectorized
    string file;
    // The vectorized result
    vector<string> vectorized;

    // Method to actually process and vectorize the file
    void process();
};

#endif
