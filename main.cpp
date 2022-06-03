#include <iostream>
#include "Finder.h"


//pass name of file as a second parameter when calling execution in command-line
int main(int argc, char**argv) {
    Finder f(8);
    fs::path result = f.getPathToTarget(std::string(argv[1]));
    return 0;
}