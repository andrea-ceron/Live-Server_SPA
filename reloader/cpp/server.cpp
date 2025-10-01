#include "../headers/server.h"
#include <sstream>
#include <algorithm>
#include <unistd.h>
#include <vector>
#include <thread>
#include "../headers/events_register.h"
#include <mutex>
#include <iostream>



ServerSSE::ServerSSE(int port){
    this->port = port;
    this->server_fd = -1;
    this->addrlen = sizeof(address);
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(port); 
}
 
ServerSSE::~ServerSSE() {
    for(int client_fd : clients){
        close(client_fd);
    }
    clients.clear();
    if (server_fd >= 0) {
        close(server_fd);
        server_fd = -1;
    }
}

bool ServerSSE::start(){
    server_fd = socket(AF_INET, SOCK_STREAM, 0); // descriptor server
    if (server_fd < 0) {
        std::cerr << "Errore: impossibile creare il socket\n";
        return false;
    }
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "Attenzione: setsockopt fallito, ma si continua.\n";
    }
    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address))< 0){ // binding socket
        std::cerr << "Errore: bind fallito\n";
        close(server_fd); 
        server_fd = -1;
        return false;    
    }
    if(listen(server_fd, 3) < 0){ // listen sul socket con max 3 connessioni in coda 
        std::cerr << "Errore: listen fallito\n";
        close(server_fd); 
        server_fd = -1;
        return false;      
    }
    std::cout << "server start()" << std::endl;


    return true;

}



void ServerSSE::acceptClients() {
    int client_fd;
    while ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) >= 0) {
        if (client_fd < 0) {
            perror("accept");
            continue; 
        }
        std::cout << "client_fd -> " << client_fd << std::endl;
        std::thread clientThread(&ServerSSE::handleClientRequest, this, client_fd);
        clientThread.detach();
        
        std::cout << "Nuova richiesta da " 
                  << inet_ntoa(address.sin_addr) << ":" 
                  << ntohs(address.sin_port) << "\n";
    }
}




void ServerSSE::handleClientRequest(int client_fd) {
    char buffer[4096] = {0};
    long valread = read(client_fd, buffer, 4096);

    
    if (valread <= 0) {
        close(client_fd);
        return;
    }

    std::string requested_path = endpoint_handler.build_request_path(buffer);
    
    if (requested_path == "/events") {
        endpoint_handler.reload_endpoint(client_fd);
        std::cout << "entrato il events" << std::endl;
        return;
    }
    if (requested_path == "/") {
        endpoint_handler.injection_endopoint(client_fd, "var/www/html/index.html" );
    } 
    close(client_fd);
}


void ServerSSE::setRootPath(const std::string& path) {
    m_html_root_dir = path; 
    if (!m_html_root_dir.empty() && m_html_root_dir.back() == '/') {
        m_html_root_dir.pop_back();
    }
}