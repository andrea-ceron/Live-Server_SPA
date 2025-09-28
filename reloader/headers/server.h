#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>         // close()
#include <sys/socket.h>     // socket(), bind(), listen(), accept()
#include <netinet/in.h>     // sockaddr_in
#include <arpa/inet.h>      // inet_ntoa()
#include <vector>
#include <thread>
#include <mutex>
#include "gateway.h"


class ServerSSE
{
    public:
        ServerSSE(int port);
        ~ServerSSE();
        bool start();
        void acceptClients();
        void setRootPath(const std::string& path); 

    private:
        int server_fd;
        struct sockaddr_in address;
        int addrlen;
        int port;
        std::string m_html_root_dir;
        Gateway endpoint_handler;
        
        std::vector<int> clients; 
        std::mutex clients_mutex; 
        void handleClientRequest(int client_fd); 
        void sendHttpResponse(int client_fd, 
                            const std::string& content, 
                            const std::string& mime_type, 
                            int status_code);
};
#endif // SERVER_H