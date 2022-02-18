#include "../../user.hpp"
#include "../../../util.hpp"

namespace crypto_com
{
    nlohmann::json UserAPI::get_deposit_history(std::string currency, double start_ts, double end_ts, int page_size, int page, std::string status)
    {
        nlohmann::json pl = nlohmann::json{
            { "method", "private/get-deposit-history" },
            { "nonce", util::create_nonce() }
        };

         pl["params"] = nlohmann::json::object();

        if (!currency.empty())
            pl["params"]["currency"] = currency;
        if (!status.empty())
            pl["params"]["status"] = status;
        if (start_ts)
            pl["params"]["start_ts"] = start_ts;
        if (end_ts)
            pl["params"]["end_ts"] = end_ts;
        if (page_size)
            pl["params"]["page_size"] = page_size;
        if (page)
            pl["params"]["page"] = page;

        util::sign_payload(pl, this->m_key, this->m_secret);

        return m_ws_client->send(pl);
    }
} // namespace crypto_com