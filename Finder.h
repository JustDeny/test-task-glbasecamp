//
// Created by jimch on 03.06.2022.
//

#ifndef PROJECT_FINDER_H
#define PROJECT_FINDER_H

#include <string>
#include <iostream>
#include <filesystem>
#include <thread>
#include <mutex>

namespace fs = std::filesystem;

class Finder {
private:
    std::mutex mutex;
    fs::path m_found_path;
    fs::path m_root_path;
    int m_num_threads = 0;
    bool complete_searching = false;
    std::string m_target;
    std::vector<std::thread> threads;
public:
    explicit Finder(int max_num_threads);
    void traverseDirs(const fs::path& path_to_dir);
    void multiThreadTraverseDirs(const fs::path& root);
    fs::path getPathToTarget(const std::string& target);
};


#endif //PROJECT_FINDER_H
