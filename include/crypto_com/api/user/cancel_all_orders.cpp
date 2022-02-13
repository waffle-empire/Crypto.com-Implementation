#include "../user.hpp"
#include "../../util.hpp"

namespace crypto_com
{
    void UserAPI::cancel_all_orders(std::string instrument_name)
    {
        nlohmann::json pl = nlohmann::json{
            { "method", "private/cancel-all-orders" },
            { "nonce", util::create_nonce() },
            { "params", {
                    { "instrument_name", instrument_name }
                }
            }
        };

        util::sign_payload(pl, this->m_key, this->m_secret);

        m_ws_client->send(pl);
    }
} // namespace crypto_com