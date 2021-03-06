#include "parse_config.h"
#include "thirdparty/json.hpp"

#include <fstream>
#include <stdexcept>

#ifdef _WIN32
#    include <Windows.h>
#else
#    include <unistd.h>
#endif

#include <filesystem>

std::string get_base_path()
{
    char buffer[512];
#ifdef _WIN32
    GetModuleFileNameA(nullptr, buffer, 512);
#else
    readlink("/proc/self/exe", buffer, 512);
#endif
    std::filesystem::path my_path = buffer;
    return my_path.remove_filename().string();
}

struct config_helper::config_helper_impl
{
    nlohmann::json json_;
    bool is_vaild_ = false;
};

config_helper::config_helper() : impl_(std::make_unique<config_helper_impl>())
{
}

config_helper::~config_helper() = default;
config_helper::config_helper(config_helper&&) noexcept = default;
config_helper& config_helper::operator=(config_helper&&) noexcept = default;

void config_helper::load_config()
{
    impl_->json_.clear();
    std::ifstream config_file(get_base_path() + config_path);

    if (!config_file)
        throw std::runtime_error("Can't find config file");

    config_file >> impl_->json_;
    impl_->is_vaild_ = true;
}

[[nodiscard]] std::string config_helper::get_host()
{
    return impl_->json_["host"].get<std::string>();
}

[[nodiscard]] int config_helper::get_port()
{
    return impl_->json_["port"].get<int>();
}

[[nodiscard]] bool config_helper::is_vaild() const noexcept
{
    return impl_->is_vaild_;
}


[[nodiscard]] std::string config_helper::get_chat_history_path()
{
    return impl_->json_["chat_history_path"].get<std::string>();
}

[[nodiscard]] std::string config_helper::get_save_files_path()
{
    return impl_->json_["save_files_path"].get<std::string>();
}
