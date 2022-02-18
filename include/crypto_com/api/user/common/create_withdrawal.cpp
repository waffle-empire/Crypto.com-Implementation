#include "../../user.hpp"
#include "../../../util.hpp"

namespace crypto_com
{
    nlohmann::json UserAPI::create_withdrawal(std::string currency, double amount, std::string address, 
                std::string client_wid, std::string address_tag, std::string network_id)
    {
        nlohmann::json pl = nlohmann::json{
            { "method", "private/create-withdrawal" },
            { "nonce", util::create_nonce() },
            { "params", {
                    { "currency", currency },
                    { "amount", amount },
                    { "address", address }
                }
            }
        };
        
        if (!client_wid.empty())
            pl["params"]["client_wid"] = client_wid;
        if (!address_tag.empty())
            pl["params"]["address_tag"] = address_tag;
        if (!network_id.empty())
            pl["params"]["network_id"] = network_id;


        util::sign_payload(pl, this->m_key, this->m_secret);

        return m_ws_client->send(pl);
    }
} // namespace crypto_com