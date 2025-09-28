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
#include "headers/server.h"
#include "headers/watcher.h"
#include "headers/gateway.h"
#include "headers/events_register.h"
#include <thread>
#include <chrono>


int main(int argc, char* argv[]){
    std::cout << "C++ Reloader App started." << std::endl;
        if (argc < 2) {
        std::cerr << "Usage: ./dev_reloader <frontend_path>\n";
        return 1;
    }

    std::string frontend_path = argv[1];
    
    ServerSSE server(8080);
    server.setRootPath(frontend_path); 

    if (!server.start()) {
        std::cerr << "ERRORE: Impossibile avviare il server sulla porta 8080.\n";
        return -1;
    }    
    // Thread 1: Server HTTP e SSE
    std::thread acceptThread([&server]() { 
        try{
            std::cout << "Server thread started, listening on port 8080.\n";
            server.acceptClients(); // Ciclo infinito
        }catch(const std::exception& e){
            std::cout << "Eccezione nel main -> "<< e.what() << std::endl;
        }
    });    
    // Thread 2: Watcher (inotify)
    Watcher watcher; 

    try {
        watcher.add_watcher_recursively(frontend_path);
        watcher.watch(); 
    } catch (const std::exception& e) {
        std::cerr << "Watcher error: " << e.what() << std::endl; 
    }

    acceptThread.join(); 
    return 0;
}