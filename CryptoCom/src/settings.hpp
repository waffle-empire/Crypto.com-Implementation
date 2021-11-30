#pragma once
#include "common.hpp"
#include <fstream>
#include "nlohmann/json.hpp"

class settings {
private:
    nlohmann::json default_options;
    nlohmann::json options;

public:
    explicit settings();
    ~settings();

    struct api
    {
        std::string key = "";
        std::string secret = "";
    };
    api api{};

    void from_json(const nlohmann::json& j);
    nlohmann::json to_json();

    void attempt_save();
    bool load();

private:
    const char* settings_location = "./settings.json";

    bool deep_compare(nlohmann::json& current_settings, const nlohmann::json& default_settings, bool compare_value = false);
    bool save();
    bool write_default_config();
};

inline settings* g_settings{};
