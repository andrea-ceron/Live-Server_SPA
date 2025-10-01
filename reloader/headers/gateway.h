#ifndef GATEWAY_H
#define GATEWAY_H

#include <string>
#include <mutex>
#include <vector>
#include <sstream>

class Gateway{
    private:
        void send_to_client(bool endpoint_flag, int client_fd, const std::string& payload_content="", const std::string& mime_type = "");
        std::string read_file_to_string(const std::string& path);
        std::vector<int> clients;       
        std::mutex clients_mutex;
        std::string get_content_type(const std::string& path);
        public:
        std::string build_request_path(char* buffer);
        void reload_endpoint(int client_fd);
        void injection_endpoint(int client_fd, const std::string& html_path);
        void serve_static_file(int client_fd, const std::string& path);
};

#endif // GATEWAY_H
