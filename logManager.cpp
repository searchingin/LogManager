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
    unsigned int temp_entry;
    std::vector<std::string> keywords;
    std::vector<int> intersection_result;
    bool is_timeStamp = false;
    bool is_category = false;
    bool is_keyword = false;
    
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
                    is_timeStamp = true;
                    is_category = false;
                    is_keyword = false;
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
                is_category = true;
                is_keyword = false;
                is_timeStamp = false;
                std::cin.get();
                std::getline(std::cin, temp);
                std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);

                os << categoryMap[temp].size() << " entries found\n";
                os << "% ";break;
            }

            // keyword search
            case 'k':{
                is_keyword = true;
                is_timeStamp = false;
                is_category = false;
                std::cin.get();
                std::getline(std::cin, temp);
                extractKeywords(keywords, temp);
                bool all_exist = true;
                for (auto it : keywords){
                    if (keywordMap.find(it) == keywordMap.end())
                        all_exist = false;
                }
                if (!all_exist)
                    os << "0 entries found\n";
                else{
                    intersection_result = keywordMap[keywords[0]];
                    std::vector<int> v;
                    for (auto it : keywords){
                         std::set_intersection(intersection_result.begin(), intersection_result.end(), keywordMap[it].begin(), keywordMap[it].end(), v.begin());
                         intersection_result = v;
                    }
                    os << intersection_result.size() << " entries found\n";
                }
                os << "% "; break;
            }

            // Append logEntry
            case 'a':{
                std::cin.get();
                unsigned int ID_temp;
                std::cin >> ID_temp;
                if (ID_temp < sortedID.size()){
                    auto it = std::find(sortedID.begin(), sortedID.end(), ID_temp);
                    unsigned int target = (unsigned int)(it - sortedID.begin());
                    excerptList.emplace_back((int)target);
                    os << "log entry " << ID_temp << " appended\n";
                }
                else 
                    std::cerr << "Error: Invalid command\n";
                os << "% "; break;
            }

            // Append previous search result to excerptList
            case 'r':{
                if (is_timeStamp){
                    unsigned int current = (unsigned int)(timeStamp_low - sortedID.begin());
                    unsigned int total = (unsigned int)(timeStamp_low - sortedID.begin());

                    while (current < total){
                        excerptList.emplace_back(current);
                        current++;
                    }
                    os << timeStamp_high - timeStamp_low << " log entries appended\n";
                }

                else if (is_category){
                    excerptList.insert(excerptList.end(), categoryMap[temp].begin(), categoryMap[temp].end());
                    os << categoryMap[temp].size() << " log entries appended\n";
                }
                
                else if (is_keyword){
                    excerptList.insert(excerptList.end(), intersection_result.begin(), intersection_result.end());
                    os << intersection_result.size() << " log entries appended\n"; 
                } 

                // No previous search
                else{
                    std::cerr << "Error: Invalid command\n";
                }

                os << "% "; break;
            }

            // Delete logEntry
            case 'd':{
                std::cin.get();
                std::cin >> temp_entry;
                
                if (temp_entry < excerptList.size()){
                    excerptList.erase(excerptList.begin() + temp_entry);
                    os << "exerpt list entry " << temp_entry << "deleted\n";
                }
                else{
                    std::cerr << "Error: Invalid command\n";
                }         

                os << "% "; break;
            }
        }
    }
}























