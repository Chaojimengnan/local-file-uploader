#pragma once
#include <memory>
#include <string>

class config_helper
{
public:
    static constexpr const char* config_path = "config.json";

    config_helper();
    ~config_helper();
    config_helper(const config_helper&) = delete;
    config_helper& operator=(const config_helper&) = delete;
    config_helper(config_helper&&) noexcept;
    config_helper& operator=(config_helper&&) noexcept;

    void load_config();
    [[nodiscard]] std::string get_host();
    [[nodiscard]] int get_port();
    [[nodiscard]] bool is_vaild() const noexcept;
    [[nodiscard]] std::string get_chat_history_path();
    [[nodiscard]] std::string get_save_files_path();

private:
    struct config_helper_impl;
    std::unique_ptr<config_helper_impl> impl_;
};