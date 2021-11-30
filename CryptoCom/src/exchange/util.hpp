#include "cryptlite/hmac.h"
#include "cryptlite/sha256.h"
#include "nlohmann/json.hpp"

namespace util
{
    inline void sign(nlohmann::json &json, std::string key, std::string secret)
    {
        uint8_t digest[cryptlite::sha256::HASH_SIZE];

        std::string params = "";
        if (json.find("params") != json.end())
            for (auto &item : json["params"].items())
                params += item.key() + to_string(item.value());

        std::string payload = json["method"].get<std::string>() + to_string(json["id"]) + key + params + to_string(json["nonce"]);

        cryptlite::hmac<cryptlite::sha256>::calc(payload, secret, digest);

        std::stringstream sstream;
        for (uint8_t i = 0u; i < cryptlite::sha256::HASH_SIZE; i++)
            sstream << std::hex << (int)digest[i];

        json["sig"] = sstream.str();
    }
}
