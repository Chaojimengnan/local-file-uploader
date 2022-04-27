#pragma once

#include <ostream>
#include <string_view>


void save_text(std::string_view chat_history_path, const std::ostream& content);
void save_file(std::string_view data, std::string_view file_name, std::string_view files_path);