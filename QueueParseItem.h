// One item in the Queue Parse List

#ifndef QUEUEPARSEITEM_H
#define QUEUEPARSEITEM_H

#include <string> // std::string

using namespace std;

class QueueSiteList{
    public:
        // Constructor that creates a queue parse item with arguments to set site and data
        QueueParseItem(string s, string d);

        // returns the data string within the object
        string getData();

        // returns site string
        string getSite();

        // sets the data string
        void setData(string d);

        //sets the site string
        void setSite(string s);

    private:
        string data;
        string site;
};

#endif
