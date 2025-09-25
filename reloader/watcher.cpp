#include <iostream>
#include <string>
#include <unordered_map>
#include <filesystem>     
#include <sys/inotify.h>   
#include <unistd.h>  



class Watcher{
    private:
        int fd;
        std::unordered_map<int, std::string> wd_to_path;
        Render* renderer;

    public:
        Watcher(Render* r) {
            this->fd = inotify_init();
            if (fd < 0) {
                perror("inotify_init");
                throw std::runtime_error("Failed to initialize inotify");
            }
            this->renderer = r;
            std::cout << "Watcher initialized." << std::endl;
        }
        ~Watcher(){
            close(fd);
            delete renderer;
        }

        void add_watcher_recursively(std::string frontend_path){
            int wd = inotify_add_watch(fd, frontend_path.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO | IN_DELETE_SELF | IN_MOVE_SELF | IN_ISDIR);
            std::cout << "Watching: " << frontend_path << std::endl;
            if (wd == -1) {
                perror("inotify_add_watch");
                return;
            }
            try{
                wd_to_path[wd] = frontend_path;
                for(const auto& entry : std::filesystem::directory_iterator(frontend_path)){
                    if(entry.is_directory()){
                        add_watcher_recursively(entry.path().string());
                    }
                    else if (entry.is_regular_file()) { 
                    int file_wd = inotify_add_watch(fd, entry.path().c_str(), 
                                                    IN_MODIFY | IN_MOVED_FROM | IN_MOVED_TO);
                    
                    if (file_wd == -1) {
                        perror("inotify_add_watch file");
                    } else {
                        wd_to_path[file_wd] = entry.path().string();
                        std::cout << "Watching file: " << entry.path().string() << std::endl;
                    }
                }
                }
            }catch(const std::filesystem::filesystem_error& e){
                std::cerr << "Filesystem error: " << e.what() << std::endl;
            }
        }

        void watch(){
            std::cout << "Starting to watch for file changes..." << std::endl;
            
            while(true){
                char buffer[8192];
                std::cout << "Waiting for events..." << std::endl;
                int length = read(fd, buffer, sizeof(buffer));
                std::cout << "Read " << length << " bytes from inotify." << std::endl;
                if (length < 0) {
                    perror("read");
                    continue;
                }
                int i = 0;
                while(i < length) {
                    std::cout << "Event detected." << std::endl;
                    struct inotify_event *event = (struct inotify_event *) &buffer[i];
                    if (event->len) {
                        std::string parent_path = wd_to_path[event->wd];
                        std::string full_path = parent_path + "/" + event->name;

                        if (event->mask & IN_CREATE) {
                            std::cout << "File created: " << event->name << std::endl;
                            add_watcher_recursively(parent_path);
                        } else if (event->mask & IN_DELETE) {
                            std::cout << "File deleted: " << event->name << std::endl;
                        } else if (event->mask & IN_MODIFY) {
                            std::cout << "File modified: " << event->name << std::endl;
                        }
                        else if (event->mask & (IN_DELETE_SELF )) {
                            std::cout << "Directory deleted/moved: " << parent_path << std::endl;
                            inotify_rm_watch(fd, event->wd);
                            wd_to_path.erase(event->wd);
                        }
                        else if (event->mask & IN_MOVED_FROM) {
                            std::cout << "File moved from: " << event->name << std::endl;
                        } else if (event->mask & IN_MOVED_TO) {
                            std::cout << "File moved to: " << event->name << std::endl;
                        }
                    }
                    i = i + sizeof(struct inotify_event) + event->len;
                }
                i =0;
            }
        }
};

