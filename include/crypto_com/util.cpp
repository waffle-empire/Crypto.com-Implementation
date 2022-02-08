#include <cryptlite/hmac.h>
#include <cryptlite/sha256.h>
#include "util.hpp"

namespace crypto_com
{
    int64_t util::create_nonce()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
    }

    void util::sign_payload(nlohmann::json& json, std::string key, std::string secret)
    {
        uint8_t digest[cryptlite::sha256::HASH_SIZE];

        std::string params = "";
        if (json.find("params") != json.end())
            for (auto &item : json["params"].items())
                params += item.key() + to_string(item.value());

        std::string payload = json["method"].get<std::string>() + to_string(json["id"]) + key + params + to_string(json["nonce"]);

        cryptlite::hmac<cryptlite::sha256>::calc(payload, secret, digest);

        std::stringstream sstream;
        sstream.fill('0');

        for (unsigned int i = 0u; i < cryptlite::sha256::HASH_SIZE; i++)
            sstream << std::hex << std::setw(2) << (int)digest[i];

        json["sig"] = sstream.str();
    }
}