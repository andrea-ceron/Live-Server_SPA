#ifndef WATCHER_H
#define WATCHER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <filesystem>     
#include <sys/inotify.h>   
#include <unistd.h>  
#include <functional>
#include "events_register.h"

class Watcher
{
    public:
        Watcher();
        ~Watcher();
        void add_watcher_recursively(std::string frontend_path);
        void watch();
    private:
        int fd;
        std::unordered_map<int, std::string> wd_to_path;
};
#endif // WATCHER_H