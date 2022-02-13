#include "../user.hpp"
#include "../../util.hpp"

namespace crypto_com
{
    void UserAPI::cancel_order(std::string instrument_name, std::string order_id)
    {
        nlohmann::json pl = nlohmann::json{
            { "method", "private/cancel-order" },
            { "nonce", util::create_nonce() },
            { "params", {
                    { "instrument_name", instrument_name },
                    { "order_id", order_id }
                }
            }
        };

        util::sign_payload(pl, this->m_key, this->m_secret);

        m_ws_client->send(pl);
    }
} // namespace crypto_com