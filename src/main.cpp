#include "file_saver.h"
#include "parse_config.h"
#include "thirdparty/httplib.h"

#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

int main()
{
    try {

        config_helper config;
        config.load_config();
        auto host = config.get_host();
        auto port = config.get_port();
        auto chat_history_path = config.get_chat_history_path();
        auto files_path = config.get_save_files_path();

        std::cout << "Listen to " << host << ":" << port << "\n";
        httplib::Server server;

        if (!server.set_mount_point("/", "./web"))
            throw std::runtime_error("Can't find directory './web'");

        server.Post("/upload_text", [&chat_history_path](const httplib::Request& req, httplib::Response& res) {
            try {
                std::stringstream out;
                out << "\n["
                    << req.remote_addr << ":" << req.remote_port << "] says:\n"
                    << req.get_file_value("text").content << "\n\n";
                std::cout << out.str();
                save_text(chat_history_path, out);
                res.set_header("upload_state", "1");
            } catch (...) {
                res.set_header("upload_state", "0");
                throw;
            }
        });

        server.Post("/upload_files", [&files_path](const httplib::Request& req, httplib::Response& res) {
            try {
                std::stringstream out;
                auto file_length = std::strtol(req.get_file_value("file_size").content.c_str(), nullptr, 10);
                for (size_t i = 0; i < file_length; i++) {
                    auto file_data = req.get_file_value((std::string("files") + std::to_string(i)).c_str());
                    out << "[" << req.remote_addr << ":" << req.remote_port << "] transfer file: "
                        << file_data.filename << "\n";
                    save_file(file_data.content, file_data.filename, files_path);
                }

                std::cout << out.str();
                res.set_header("upload_state", "1");
            } catch (...) {
                res.set_header("upload_state", "0");
                throw;
            }
        });

        server.listen(host.c_str(), port);

        std::cout << "Server exit\n";

    } catch (const std::exception& e) {
        std::cout << "Error occurs : " << e.what() << "\n";
    }
    return 0;
}