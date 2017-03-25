
// implementation of the QueueParseItem class members

 #include "QueueParseItem.h"

 // Constructor
 QueueParseItem::QueueParseItem(string s, string d) {
    data = d;
    site = s;
 }

 string QueueParseItem::getData() {
    return data;
 }

 string QueueParseItem::getSite() {
    return site;
 }

 void QueueParseItem::setData(string d) {
    data = d;
 }

 void QueueParseItem::setSite(string s) {
    site = s;
 }
