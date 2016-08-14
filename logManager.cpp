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
        std::vector<int> &sortedID){
    for (unsigned int i = 0; i < masterFile->size(); ++i){
        sortedID.emplace_back((*masterFile)[i].entryID);
    }
    comparator comp(masterFile);
    // Sort the entryID using O(nlog(n))
    std::sort(sortedID.begin(), sortedID.end(), comp);
    // First build categoryMap
    std::string temp;
    std::vector<std::string> keywords;
    for (unsigned int i = 0; i < sortedID.size(); ++i){
        temp = (*masterFile)[(unsigned int)sortedID[i]].category;
        std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        categoryMap[temp].emplace_back(i);
        // Put category keywords into keywordMap
        keywords.clear();
        extractKeywords(keywords, (*masterFile)[(unsigned int)sortedID[i]].category);

        for (auto it : keywords){
            if (keywordMap[it].empty())
                keywordMap[it].emplace_back(i);
            else if (keywordMap[it].back() != (int)i)
                keywordMap[it].emplace_back(i);
        }
        // Put message keywords into keywordMap
        keywords.clear();
        extractKeywords(keywords, (*masterFile)[(unsigned int)sortedID[i]].message);

        for (auto it : keywords){
            if (keywordMap[it].empty())
                keywordMap[it].emplace_back(i);
            else if (keywordMap[it].back() != (int)i)
                keywordMap[it].emplace_back(i);
        } 
    }    
}

// Read commands and output results to command line or redirected file
// EFFECTS: construct excerptList and implement searching and editing of excerptList
void logman::cmdOpt(std::ostringstream &os,
        std::unordered_map<std::string, std::vector<int>> &categoryMap,
        std::unordered_map<std::string, std::vector<int>> &keywordMap,
        std::vector<logEntry> *masterFile,
        std::vector<int> &sortedID,
        std::deque<int> &excerptList){
    std::vector<int>::iterator timeStamp_low;
    std::vector<int>::iterator timeStamp_high;
    std::string temp;
    std::vector<std::string> keywords;
    os << "% ";
    char cmd;
    while (std::cin >> cmd){
        switch (cmd){
            // timeStamp search
            case 't':{
                std::cin.get();
                std::string begin, end;
                std::getline(std::cin, begin, '|');
                std::cin >> end;
                timeStamp_comparator cmp(masterFile);

                if (begin.length() == 14 && end.length() == 14){
                    timeStamp_low = std::lower_bound(sortedID.begin(), sortedID.end(), begin, cmp);
                    timeStamp_high = std::upper_bound(sortedID.begin(), sortedID.end(), end, cmp);

                    os << timeStamp_high - timeStamp_low << " entries found\n";
                }
                else{
                    std::cerr << "Error: Invalid command\n";
                }

                os << "% "; break;
                     }

            // category search
            case 'c':{
                std::cin.get();
                std::getline(std::cin, temp);
                std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);

                os << categoryMap[temp].size() << " entries found\n";
                os << "% ";break;
                     }

            // keyword search
            case 'k':
                std::cin.get();
                std::getline(std::cin, temp);
                extractKeywords(keywords, temp);
                bool all_exist = true;
                for (auto it : keywords){
                    if (keywordMap.find(it) == keywordMap.end())
                        all_exist = false;
                }
                if (!all_exist)
                    os << "0 entries found\n" << "% ";
        }
    }
}























