#include "../../user.hpp"
#include "../../../util.hpp"

namespace crypto_com
{
    nlohmann::json UserAPI::get_currency_networks()
    {
        nlohmann::json pl = nlohmann::json{
            { "method", "private/get-currency-networks" },
            { "nonce", util::create_nonce() }
        };

        pl["params"] = nlohmann::json::object();
        
        util::sign_payload(pl, this->m_key, this->m_secret);

        return m_ws_client->send(pl);
    }
} // namespace crypto_com