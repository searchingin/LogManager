/* eecs281-project3-log manager 
 *
 * Copyright @ YI YANG(yeeyoung@umich.edu), all rights reserved
 *
 * proj3.cpp
 */
#include <iostream>
#include <sstream>

// This program is interactive, but the autograder don't run this program interactively.
// Therefore, we can use ostringstream throughout the whole project to speed up the program

void runProgram(std::ostream& out){
    // Run your main program
    out << "Hello, world!\n";
}

int main(){
#ifndef DEBUG
    std::ostringstream oss;
    std::ostream& out = oss;
#else
    std::ostream& out = std::cout;
#endif

    runProgram(out);

#ifndef DEBUG
    std::cout << oss.str();
#endif
}
