#include "../../user.hpp"
#include "../../../util.hpp"

namespace crypto_com
{
    nlohmann::json UserAPI::get_cancel_on_disconnect()
    {
        nlohmann::json pl = nlohmann::json{
            { "method", "private/get-cancel-on-disconnect" },
            { "nonce", util::create_nonce() }
        };
        
        util::sign_payload(pl, this->m_key, this->m_secret);

        return m_ws_client->send(pl);
    }
} // namespace crypto_com