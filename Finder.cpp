//
// Created by jimch on 03.06.2022.
//

#include "Finder.h"

Finder::Finder(int max_num_threads):
    m_num_threads{max_num_threads}, m_root_path(fs::current_path().root_path())
{}

void Finder::traverseDirs(const fs::path &path_to_dir) {
    //try/catch to prevent terminating program by failing to open window's inaccessible files
    try {
        bool found = false;
        fs::directory_iterator dir_iter(path_to_dir, fs::directory_options::skip_permission_denied);
        for (auto &entry: dir_iter) {
            if(m_num_threads > 1 || found)
            {
                std::lock_guard<std::mutex> guard(mutex);
                if(complete_searching)
                {
                    break;
                }
                if(found)
                {
                    complete_searching = true;
                    m_root_path = entry.path();
                    std::cout << m_root_path << std::endl;
                    break;
                }
            }
            auto p = fs::status(entry).permissions();
            if ((p & fs::perms::owner_read) == fs::perms::none)
                continue;
            if (entry.is_directory()) {
                if (entry.exists()) {
                    traverseDirs(entry.path());
                }
            }
            else
            {
                if(entry.path().filename() == m_target)
                {
                    found = true;
                    m_found_path = entry.path();
                }
            }
        }
    }
    catch(fs::filesystem_error& err)
    {
        return;
    }
}

fs::path Finder::getPathToTarget(const std::string& target) {
    m_target = target;
    multiThreadTraverseDirs(m_root_path);
    if(complete_searching)
        return m_found_path;
    return m_root_path;
}

void Finder::multiThreadTraverseDirs(const fs::path &root) {
    int available_threads = m_num_threads;
    fs::directory_iterator dir_iter(root, fs::directory_options::skip_permission_denied);
    bool found = false;
    for(auto& entry: dir_iter) {
        if(entry.exists() && entry.is_regular_file() && entry.path().filename() == m_target)
        {
            found = true;
        }
        else if(entry.is_directory()) {
            if (available_threads > 1) {
                threads.emplace_back(std::thread{&Finder::traverseDirs, this, entry});
                available_threads--;
            } else {
                traverseDirs(entry);
            }
        }
        {
            std::lock_guard<std::mutex> guard(mutex);
            if(complete_searching)
            {
                break;
            }
            else if(found)
            {
                m_root_path = entry.path();
                std::cout << m_root_path << std::endl;
                complete_searching = true;
                break;
            }
        }
    }
    for (auto& thr : threads)
    {
        if (thr.joinable())
            thr.join();
    }
    threads.clear();
}

