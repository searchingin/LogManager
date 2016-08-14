/* eecs281-project3-log manager
 *
 * Copyright @ YI YANG(yeeyoung@umich.edu), all rights reserved
 *
 * logManager.cpp
 */
#include "logManager.h"

// Implementation of member functions for logman class
void logman::readMaster(const char* filename, std::vector<logEntry> *masterFile, std::ostringstream &os){
    std::ifstream fin;
    fin.open(filename);
    if (!fin.is_open()){
        std::cerr << "File could not be opened\n";
        exit(0);
    }
    // If file is opened
    unsigned int count = 0;
    logEntry current;
    while (std::getline(fin, current.timeStamp, '|')){
        std::getline(fin, current.category, '|');
        std::getline(fin, current.message);
        current.entryID = count++;
        masterFile->emplace_back(current);
    }
    fin.close();
    os << count << " entries read\n";
}

// Extract keywords from category/message/keyword search message
// Function: transform upper-case alphabetics to lower-case and extract all alphanumeric characters
void logman::extractKeywords(std::vector<std::string> &keywords, std::string string_in){
    unsigned int length = 0;
    std::string temp;
    for (unsigned int i = 0; i < string_in.length(); ++i){
        string_in[i] = (char)tolower(string_in[i]);
        if (isalnum(string_in[i]))
            length++;
        else{
            if (length){
                temp = string_in.substr(i - length, length);
                keywords.emplace_back(temp);
                length = 0;
            }
        }
        // emplace_back the last word into keywords container
        if (length){
            temp = string_in.substr(string_in.length() - length, length);
            keywords.emplace_back(temp);
        }
    }
}

// Build category_map and keyword_map
// EFFECTS: put all presorted IDS into unordered map
void logman::buildMap(std::unordered_map<std::string, std::vector<int>> &categoryMap,
        std::unordered_map<std::string, std::vector<int>> &keywordMap,
        std::vector<logEntry> *masterFile,
        std::vector<int> &sortedID,
        std::deque<int> &excerptList){
    for (unsigned int i = 0; i < masterFile->size(); ++i){
        sortedID.emplace_back((*masterFile)[i].entryID);
    }
    comparator comp(masterFile);
    // Sort the entryID using O(nlog(n))
    std::sort(sortedID.begin(), sortedID.end(), comp);
    // First build categoryMap
    std::string temp;
    for (unsigned int i = 0; i < sortedID.size(); ++i){
        temp = (*masterFile)[i].category;
        std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        categoryMap[temp].emplace_back(i);
    }    
}




