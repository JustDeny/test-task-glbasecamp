#include <string>
#include <iostream>
#include <filesystem>
#include "Finder.h"


int main(int argc, char**argv) {
    Finder f(8);
    fs::path result = f.getPathToTarget(std::string(argv[1]));
    std::cout << "Result = " << result;
    std::cin.get();
    return 0;
}