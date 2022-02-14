#include "../user.hpp"
#include "../../util.hpp"

namespace crypto_com
{
    nlohmann::json UserAPI::get_open_orders(std::string instrument_name, int page_size, int page)
    {
        nlohmann::json pl = nlohmann::json{
            { "method", "private/get-open-orders" },
            { "nonce", util::create_nonce() }
        };

        pl["params"] = nlohmann::json::object();

        if (!instrument_name.empty())
            pl["params"]["instrument_name"] = instrument_name;
        if (page_size)
            pl["params"]["page_size"] = page_size;
        if (page)
            pl["params"]["page"] = page;


        util::sign_payload(pl, this->m_key, this->m_secret);

        return m_ws_client->send(pl);
    }
} // namespace crypto_com