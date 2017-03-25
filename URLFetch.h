// Fetches a URL via libcurl and copies it to a new string

#ifndef URLFETCH_H
#define URLFETCH_H

#include <string> // std::string
#include <curl/curl.h> // libcurl

using namespace std;

class URLFetch{
public:
    // Constructor
    URLFetch(string URL);

    // Fetches the url
    string fetch();

private:
    string url; // URL to be fetched
    string data; // The fetched data

    // Initialization of the libcurl connection
    bool init(CURL *&conn);
};

#endif
