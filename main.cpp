#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <queue>
#include <unordered_map>
#include <filesystem>     
#include <sys/inotify.h>   
#include <unistd.h>  
class Render{
    private:
        std::unordered_map<std::string, std::string> cache;

    public:
        Render(std::string path){
            for(const auto& entry : std::filesystem::recursive_directory_iterator(path)){
                if(entry.is_regular_file()){
                    addFile(entry.path().string());
                }
            }
        }
        ~Render(){
            cache.clear();
        }

        void addFile(std::string frontend_path){
            std::ifstream file(frontend_path);
            if(!file.is_open()){
                std::cerr << "Error opening file: " << frontend_path << std::endl;
                return;
            }
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            cache[frontend_path] = content;
            file.close();
        }

        void eraseFile(std::string frontend_path){
            cache.erase(frontend_path);
        }

};

class Watcher{
    private:
        int fd;
        std::unordered_map<int, std::string> wd_to_path;
        Render* renderer;

    public:
        Watcher(Render* r) {
            int fd = inotify_init();
            if (fd < 0) {
                perror("inotify_init");
                throw std::runtime_error("Failed to initialize inotify");
            }
            this->renderer = r;
        }
        ~Watcher(){
            close(fd);
            delete renderer;
        }

        void add_watcher_recursively(std::string frontend_path){
            int wd = inotify_add_watch(fd, frontend_path.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE);
            if (wd == -1) {
                perror("inotify_add_watch");
                return;
            }
            wd_to_path[wd] = frontend_path;
            for(const auto& entry : std::filesystem::directory_iterator(frontend_path)){
                if(entry.is_directory()){
                    add_watcher_recursively(entry.path().string());
                }
            }
        }

        void watch(){
            
            int i = 0;
            while(true){
                char buffer[8192];
                int length = read(fd, buffer, sizeof(buffer));
                if (length < 0) {
                    perror("read");
                    continue;
                }
                while(i < length) {
                    struct inotify_event *event = (struct inotify_event *) &buffer[i];
                    if (event->len) {
                        std::string parent_path = wd_to_path[event->wd];
                        std::string full_path = parent_path + "/" + event->name;

                        if (event->mask & IN_CREATE) {
                            std::cout << "File created: " << event->name << std::endl;
                            add_watcher_recursively(parent_path);
                            renderer->addFile(full_path);
                        } else if (event->mask & IN_DELETE) {
                            std::cout << "File deleted: " << event->name << std::endl;
                            renderer->eraseFile(full_path);
                        } else if (event->mask & IN_MODIFY) {
                            std::cout << "File modified: " << event->name << std::endl;
                            renderer->addFile(full_path);
                        }
                        else if (event->mask & (IN_DELETE_SELF )) {
                            std::cout << "Directory deleted/moved: " << parent_path << std::endl;
                            inotify_rm_watch(fd, event->wd);
                            wd_to_path.erase(event->wd);
                        }
                        else if (event->mask & IN_MOVED_FROM) {
                            std::cout << "File moved from: " << event->name << std::endl;
                            renderer->eraseFile(full_path);
                        } else if (event->mask & IN_MOVED_TO) {
                            std::cout << "File moved to: " << event->name << std::endl;
                            renderer->addFile(full_path);
                        }
                    }
                    i = i + sizeof(struct inotify_event) + event->len;
                }
                i =0;
            }
        }
};



int main(int argc, char* argv[]){
        if (argc < 2) {
        std::cerr << "Usage: ./dev_reloader <frontend_path>\n";
        return 1;
    }

    std::string frontend_path = argv[1];
    std::cout << "Frontend path: " << frontend_path << std::endl;
    // Render* renderer = new Render(frontend_path);
    // Watcher* watcher = new Watcher(renderer);
    // watcher->add_watcher_recursively(frontend_path);
    // watcher->watch();
    
    return 0;
}