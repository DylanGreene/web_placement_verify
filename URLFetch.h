// Fetches a URL via libcurl and copies it to a new string

#ifndef URLFETCH_H
#define URLFETCH_H

#include <string> // std::string

using namespace std;

class URLFetch{
public:
    // Constructor
    URLFetch(string url);

    // Fetches the url
    string fetch();

private:
    // URL to be fetched
    string URL;

    // Write callback function used to download the data into a string
    size_t WriteStringCallback(void *contents, size_t size, size_t nmemb, void *userp);
};

#endif
