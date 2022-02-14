#include "../user.hpp"
#include "../../util.hpp"

namespace crypto_com
{
    nlohmann::json UserAPI::get_account_summary(std::string currency)
    {
        nlohmann::json pl = nlohmann::json{
            { "method", "private/get-account-summary" },
            { "nonce", util::create_nonce() }
        };

        pl["params"] = nlohmann::json::object();

        if (currency.empty())
            pl["params"] = nlohmann::json::object();
        else
            pl["params"] = nlohmann::json{ { "currency", currency } };

        util::sign_payload(pl, this->m_key, this->m_secret);

        return m_ws_client->send(pl);
    }
} // namespace crypto_com