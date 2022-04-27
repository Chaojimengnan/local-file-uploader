#include "file_saver.h"
#include <fstream>
#include <ios>
#include <ostream>

void save_text(std::string_view chat_history_path, const std::ostream& content)
{
    std::ofstream file_out(chat_history_path.data(), std::ios_base::app);
    if (!file_out)
        throw std::runtime_error("Open chat_history file fails!");

    file_out << content.rdbuf();
}

void save_file(std::string_view data, std::string_view file_name, std::string_view files_path)
{
    std::ofstream file_out(std::string(files_path) + file_name.data(), std::ios_base::out | std::ios_base::binary);
    if (!file_out)
        throw std::runtime_error("Create file fails!");

    file_out.write(data.data(), static_cast<std::streamsize>(data.size()));
}