/* eecs281-project3-log manager 
 *
 * Copyright @ YI YANG(yeeyoung@umich.edu), all rights reserved
 *
 * proj3.cpp
 */
#include <iostream>
#include "logManager.h"
#include <getopt.h>

// This program is interactive, but the autograder don't run this program interactively.
// Therefore, we can use ostringstream throughout the whole project to speed up the program

void runProgram(std::ostream& out, const char *filename){
    // Run your main program
    logman manager;
    std::vector<logEntry> *masterFile = nullptr;;
    manager.readMaster(filename, masterFile, out);
    std::unordered_map<std::string, std::vector<int>> categoryMap;
    std::unordered_map<std::string, std::vector<int>> keywordMap;
    std::vector<int> sortedID;
    std::deque<int> excerptList;
    manager.buildMap(categoryMap, keywordMap, masterFile, sortedID);
    manager.cmdOpt(out, categoryMap, keywordMap, masterFile, sortedID, excerptList);
}

int main(int argc, char *argv[]){
    std::string argv1(argv[1]);
    if (argc < 2){
        std::cerr << "Error: Invalid command\n";
    }
    if (argv1 == "-h" || argv1 == "--help"){
        std::cout << "SOME RANDOM HELP STATEMENT\n";
        exit(1);
    }
#ifndef DEBUG
    std::ostringstream oss;
    std::ostream& out = oss;
#else
    std::ostream& out = std::cout;
#endif

    runProgram(out, argv[1]);

#ifndef DEBUG
    std::cout << oss.str();
    std::exit(EXIT_SUCCESS);
#endif
}
