// Fetches a URL via libcurl and copies it to a new string

#include "URLFetch.h"

#include <iostream>

// Constructor
URLFetch::URLFetch(string URL){
    url = URL;
}

// Fetch the URL and return a string of the data
string URLFetch::fetch(){
    CURL *curl;
    CURLcode res;

    // Make sure the libcurl connnection inits successfully
    if(init(curl)){
        // Get the URL!
        res = curl_easy_perform(curl);

        // Check for errors
        if(res != CURLE_OK){
            cerr << "Failed on curl_easy_perform: " << curl_easy_strerror(res);
        }else{
            return data;
        }

        // Cleanup curl stuff
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }

    return "";
}

// Write callback function used to download the data into a string
static size_t WriteStringCallback(void *contents, size_t size, size_t nmemb, void *userp){
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// libcurl connection initialization
bool URLFetch::init(CURL *&conn){
    CURLcode code;
    conn = curl_easy_init();
    if(conn){
        code = curl_easy_setopt(conn, CURLOPT_URL, url.c_str());
        if(code != CURLE_OK) {
            cerr << "Failed to set URL" << endl;
            return false;
        }

        code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, WriteStringCallback);
        if(code != CURLE_OK) {
            cerr << "Failed to set writer" << endl;
            return false;
        }

        code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, &data);
        if(code != CURLE_OK) {
            cerr << "Failed to set write data" << endl;
            return false;
        }
    }else{
        cerr << "Failed to create CURL connection" << endl;
        return false;
    }

    return true;
}



int main(int argc, char *argv[]){
    URLFetch url("http://www.google.com");
    cout << url.fetch() << endl;
}
