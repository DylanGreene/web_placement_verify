// Fetches a URL via libcurl and copies it to a new string

#include "URLFetch.h"

#include <curl/curl.h> // libcurl
#include <iostream>

// Constructor
URLFetch::URLFetch(string URL){
    url = URL;
}

string URLFetch::fetch(){
    CURL *curl;
    CURLcode res;
    string data;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteStringCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        return data;
    }
}

// libcurl connection initialization
bool init(CURL *&conn){
    
    return true;
}

size_t URLFetch::WriteStringCallback(void *contents, size_t size, size_t nmemb, void *userp){
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
