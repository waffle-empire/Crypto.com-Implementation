#include "../../user.hpp"
#include "../../../util.hpp"

namespace crypto_com
{
    nlohmann::json UserAPI::set_cancel_on_disconnect(std::string scope)
    {
        nlohmann::json pl = nlohmann::json{
            { "method", "private/set-cancel-on-disconnect" },
            { "nonce", util::create_nonce() },
            { "params", {
                    { "scope", scope }
                }
            }
        };
        
        util::sign_payload(pl, this->m_key, this->m_secret);

        return m_ws_client->send(pl);
    }
} // namespace crypto_com