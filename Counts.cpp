// implementation of C ounts class members
// inputs a string of data and a list of search words and returns a map of the words and their Counts

#include "Counts.h"
#include <iostream>

using namespace std;

// Constructor
Counts::Counts() {
}

// returns the hashtable of current counts
unordered_map<string, int> Counts::getCounts() {
    return counts;
}

// inputs a data string and a vector of search words and returns a map with
// the counts of those search words
void Counts::createCounts(string data, vector<string> searchWords) {
    for(auto it = searchWords.begin(); it != searchWords.end(); ++it){
        int count = 0;
        size_t found = data.find(*it);
        while(found != string::npos){
            count++;
            found = data.find(*it, found+1);
        }
        // insert word and count into hash table
        counts[*it] = count;
    }
}

// returns the count of searchWords
int Counts::getCount(string searchWord) {
    // check if searchWord in counts
    auto found = counts.find(searchWord);

    // if not found, return 0
    if (found == counts.end()) {
        return 0;
    }

    // if found, return
    return counts[searchWord];
}
