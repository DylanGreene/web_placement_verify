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
    int i;
    for (i = 0; i < searchWords.size(); i++) {
        int c = 0;

        // find first occurance of the word
        size_t nPos = data.find(searchWords[i], 0);

        // whie not end of string, keep looking for searchword in data
        while (nPos != string::npos) {
            c++;
            nPos = data.find(searchWords[i], nPos+1);
        }

        // insert word and count into map
        counts[searchWords[i]] = c;
    }
}

// returns the count of searchWords
int Counts::getCount(string searchWord) {
    // check if searchWord in counts
    std::unordered_map<string, int>::const_iterator found = counts.find(searchWord);

    // if not found, return 0
    if (found == counts.end()) {
        return 0;
    }

    // if found, return
    return counts[searchWord];
}

// sets the count of searchWord to c
void Counts::setCount(string searchWord, int c) {
    counts[searchWord] = c;
}

// increments/decrements count of search word by 1
void Counts::increment(string searchWord) {
    counts[searchWord] = counts[searchWord] + 1;
}

void Counts::decrement(string searchWord) {
    counts[searchWord] = counts[searchWord] - 1;
}
