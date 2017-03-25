// Takes in data from curl and search words and searches for those words and their counts

#ifndef COUNTS_H
#define COUNTS_H

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Counts {
    public:
        // Constructor
        Counts();

        // returns the hashtable of current counts
        unordered_map<string, int> getCounts();

        // inputs a data string and a vector of search words and returns a map with
        // the counts of those search words
        void createCounts(string data, vector<string> searchwords);

        // returns the count of searchWord
        int getCount(string searchWord);

        // sets the count of searchWord to c
        // TODO: is this necessary??
        void setCount(string searchWord, int c);

        // increments/decrements count of searchWord by 1
        void increment(string searchWord);
        void decrement(string searchWord);

    private:
        unordered_map<string, int> counts;


};


#endif
