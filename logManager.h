/* eecs281-project3-log manager
 *
 * Copyright @ YI YANG(yeeyoung@umich.edu), all rights reserved
 *
 * logManager.h
 */
#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <string>
#include <iostream>
#include <vector>
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




#endif
