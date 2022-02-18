#include "../../user.hpp"
#include "../../../util.hpp"

namespace crypto_com
{
    nlohmann::json UserAPI::get_instruments()
    {
        nlohmann::json pl = nlohmann::json{
            { "method", "private/create-withdrawal" },
            { "nonce", util::create_nonce() }
        };

        util::sign_payload(pl, this->m_key, this->m_secret);

        return m_ws_client->send(pl);
    }
} // namespace crypto_com