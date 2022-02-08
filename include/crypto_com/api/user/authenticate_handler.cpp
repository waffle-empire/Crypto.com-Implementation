#include "../user.hpp"
#include "../../util.hpp"

namespace crypto_com
{
    void UserAPI::authenticate_handler()
    {
        nlohmann::json pl = nlohmann::json{
            { "api_key", this->m_key },
            { "method", "public/auth" },
            { "nonce", util::create_nonce() }
        };

        util::sign_payload(pl, this->m_key, this->m_secret);

        m_ws_client->send(pl);
    }
} // namespace crypto_com
