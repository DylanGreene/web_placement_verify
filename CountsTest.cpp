#include "Counts.h"
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

int main() {
    Counts test;
    unordered_map<string, int> output;

    string s = "Hello there my name is Borah. This is my OS homework. I hate homework and OS. onit.";
    vector<string> testwords = {"my", "is", "onit"};
    test.createCounts(s, testwords);

    int i;
    for (i=0; i<testwords.size(); i++) {
        cout << testwords[i] << " occurs " << test.getCount(testwords[i]) << " times." << endl;
    }

}
