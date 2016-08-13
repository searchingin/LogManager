/* eecs281-project3-log manager
 *
 * Copyright @ YI YANG(yeeyoung@umich.edu), all rights reserved
 *
 * logManager.h
 */
#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <unordered_map>
// Basic data structure to represent log entry
struct logEntry{
    unsigned int entryID;
    std::string timeStamp;
    std::string category;
    std::string message;
};

// Comparator used to sort log entry by timestamp/category/entryID
class comparator{
private:
    std::vector<logEntry> *masterFile;
public:
    comparator(std::vector<logEntry> *masterFile_in): masterFile(masterFile_in){}
    // Operator overloading
    bool operator()(const logEntry& left, const logEntry& right) const{
        if (left.timeStamp < right.timeStamp)
            return true;
        else if (left.timeStamp == right.timeStamp && left.category < right.category)
            return true;
        else if (left.timeStamp == right.timeStamp && left.category == right.category && left.entryID < right.entryID)
            return true;
        else
            return false;
    }
};

// Main program starts here
class logman{
public:
    // Read log entries from master file
    void readMaster(const char* filename, std::vector<logEntry> *masterFile);

    // Extract keywords from category/message/keyword search message
    void extractKeywords(std::vector<std::string> &keywords, std::string);

    // Build category_map and keyword_map
    void buildMap(std::unordered_map<std::string, std::vector<int>> &categoryMap, std::unordered_map<std::string, std::vector<int>> &keywordMap, 
            std::vector<logEntry> *masterFile, std::vector<int> &sortedID, std::deque<int> &excerptList);

    // Read commands and output results to command line or redirected file
    void cmdOpt(std::ostringstream &os, 
            std::unordered_map<std::string, std::vector<int>> &categoryMap,
            std::unordered_map<std::string, std::vector<int>> &keywordMap,
            std::vector<logEntry> *masterFile,
            std::vector<int> &sortedID,
            std::deque<int> &excerptList);
};



#endif
