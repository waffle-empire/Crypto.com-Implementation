#include "../../user.hpp"
#include "../../../util.hpp"

namespace crypto_com
{
    nlohmann::json UserAPI::get_deposit_address(std::string currency)
    {
        nlohmann::json pl = nlohmann::json{
            { "method", "private/get-deposit-address" },
            { "nonce", util::create_nonce() },
            { "params", {
                { "currency", currency }
            }}
        };

        util::sign_payload(pl, this->m_key, this->m_secret);

        return m_ws_client->send(pl);
    }
} // namespace crypto_com