#pragma once
#include "common.hpp"

namespace crypto_com
{
    struct util
    {
        static int64_t create_nonce();
        static void sign_payload(nlohmann::json& json, std::string key, std::string secret);
    };
}