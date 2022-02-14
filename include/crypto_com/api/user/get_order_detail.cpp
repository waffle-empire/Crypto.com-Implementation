#include "../user.hpp"
#include "../../util.hpp"

namespace crypto_com
{
    nlohmann::json UserAPI::get_order_detail(std::string order_id)
    {
        nlohmann::json pl = nlohmann::json{
            { "method", "private/get-order-detail" },
            { "nonce", util::create_nonce() },
            {
                "params", {
                    {"order_id", order_id}
                }
            }
        };
        
        util::sign_payload(pl, this->m_key, this->m_secret);

        return m_ws_client->send(pl);
    }
} // namespace crypto_com