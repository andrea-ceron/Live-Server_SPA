#include "../headers/gateway.h"
#include "../headers/events_register.h"
#include <fstream>   
#include <sstream>   
#include <sys/socket.h> 
#include <unistd.h>  
#include <iostream>
#include <stdexcept>


std::string Gateway::build_request_path(char* buffer){
    std::string request(buffer);
    
    size_t start = request.find("GET ") + 4;
    size_t end = request.find(" HTTP/");
    std::string requested_path = "/"; 
    
    if (start != std::string::npos && end != std::string::npos && end > start) {
        requested_path = request.substr(start, end - start);
    }
    
    size_t query_pos = requested_path.find('?');
    if (query_pos != std::string::npos) {
        requested_path = requested_path.substr(0, query_pos);
    }
    return requested_path;
}

void Gateway::reload_endpoint(int client_fd){
    const std::string sse_headers =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/event-stream\r\n"
        "Cache-Control: no-cache\r\n"
        "Connection: keep-alive\r\n\r\n";
        
    ssize_t sent = send(client_fd, sse_headers.c_str(), sse_headers.length(), 0);
    if (sent == -1) {
        std::cerr << "Errore invio header SSE al client " << client_fd << std::endl;
        return;
    }

    while (true) {
        bool event_happened = events_register::getInstance().read_event();

        if (event_happened) {
            const std::string sse_message = "data: reload\n\n";
            ssize_t bytes_sent = send(client_fd, sse_message.c_str(), sse_message.length(), 0);

            if (bytes_sent == -1) {
                std::cerr << "Client " << client_fd << " disconnesso durante l'invio dell'evento." << std::endl;
                break; 
            }
            std::cout << "Evento di ricarica inviato a client " << client_fd << std::endl;

        } else {
            usleep(500000); 
            const std::string heartbeat = ":heartbeat\n\n";
            send(client_fd, heartbeat.c_str(), heartbeat.length(), MSG_NOSIGNAL);
        }
        char buffer[1];
        if (recv(client_fd, buffer, 1, MSG_PEEK | MSG_DONTWAIT) == 0 && errno != EAGAIN) {
             std::cout << "Client " << client_fd << " ha chiuso la connessione (recv check)." << std::endl;
             break;
        }
    }

    close(client_fd);
    std::cout << "Connessione SSE terminata per client " << client_fd << std::endl;
}

void Gateway::injection_endpoint(int client_fd, const std::string& html_path){
    std::cout << html_path << std::endl;
    std::string html_template = this->read_file_to_string(html_path);

    const std::string sse_js_code = R"(
        (function() {
            const eventSource = new EventSource('/events');
            eventSource.onmessage = function(event) {
                if (event.data === 'reload') {
                    console.log('Live Reload: Rilevato cambiamento, ricaricamento pagina...');
                    window.location.reload();
                }
            };
            eventSource.onerror = function(err) {
                console.error('Live Reload: Errore connessione SSE.');
                eventSource.close();
                // Implementare una logica di riconnessione qui è consigliato
            };
        })();
        )";
    const std::string script_tag = "\n<script>" + sse_js_code + "</script>\n";
        std::string::size_type pos = html_template.find("</body>");
    if (pos != std::string::npos) {
        html_template.insert(pos, script_tag);
    } else {
        html_template += script_tag;
    }
    bool full_http_header = false;
    this->send_to_client(full_http_header, client_fd, html_template, "text/html");
    close(client_fd); 

}

void Gateway::serve_static_file(int client_fd, const std::string& path) {
    try {
        if (path.starts_with("/.well-known/")) {
            close(client_fd); // chiudi silenziosamente
            return;
        }
        std::cout << "entro in static_file "<< path << std::endl;
        std::string file_content = read_file_to_string(path);
        std::string content_type = get_content_type(path);
        bool full_http_header = false;
        std::cout << "content type "<< content_type << std::endl;
        send_to_client(full_http_header, client_fd, file_content, content_type);
    } catch (const std::exception& e) {
        std::cerr << "Errore nel servire file statico (" << path << "): " << e.what() << std::endl;
        std::string error_response =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n\r\n"
            "File not found";
        send(client_fd, error_response.c_str(), error_response.size(), 0);
    }
    close(client_fd);
}

std::string Gateway::get_content_type(const std::string& path) {
    std::cout << "get_content_type" << path << std::endl;
    if (path.ends_with(".html")) return "text/html";
    if (path.ends_with(".js"))   return "application/javascript";
    if (path.ends_with(".css"))  return "text/css";
    if (path.ends_with(".png"))  return "image/png";
    if (path.ends_with(".jpg") || path.ends_with(".jpeg")) return "image/jpeg";
    if (path.ends_with(".svg"))  return "image/svg+xml";
    if (path.ends_with(".ico"))  return "image/x-icon";  // favicon

    throw std::runtime_error("Nessun content-type trovato per il file: " + path);
}


std::string Gateway::read_file_to_string(const std::string& path) {

    std::ifstream file(path, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Nessun file trovato: " + path);
    }

    std::ostringstream ss;
    ss << file.rdbuf();

    return ss.str();
} 


void Gateway::send_to_client(bool endpoint_flag, int client_fd, const std::string& payload_content, const std::string& mime_type){
    if(endpoint_flag){
        std::string header = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/event-stream\r\n"
            "Cache-Control: no-cache\r\n"
            "Connection: keep-alive\r\n\r\n";
        send(client_fd, header.c_str(), header.size(), 0);
        std::string msg = "data: reload\n\n";  
        send(client_fd, msg.c_str(), msg.size(), 0);
    }
    else{
        size_t content_length = payload_content.length();
        std::stringstream header_stream;
        header_stream << "HTTP/1.1 200 OK\r\n"; 
        header_stream << "Content-Type: " << mime_type << "\r\n";
        header_stream << "Content-Length: " << content_length << "\r\n";
        header_stream << "Connection: close\r\n";
        
        header_stream << "\r\n"; 
           std::string header_string = header_stream.str();

        std::cout << "Contenuto di header_stream:\n";
        std::cout << header_stream.str()<<std::endl; 


        send(client_fd, header_string.c_str(), header_string.length(), 0); // CORRETTO
        send(client_fd, payload_content.c_str(), payload_content.length(), 0);

    }
}