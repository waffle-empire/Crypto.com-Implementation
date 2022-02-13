#include "../user.hpp"
#include "../../util.hpp"

namespace crypto_com
{
    void UserAPI::create_order(std::string instrument_name, std::string side, std::string type,
                double price, double quantity, double notional,
                double trigger_price, std::string client_oid,
                std::string time_in_force, std::string exec_inst)
    {
        nlohmann::json pl = nlohmann::json{
            { "method", "private/create-order" },
            { "nonce", util::create_nonce() },
            { "params", {
                    { "instrument_name", instrument_name },
                    { "side", side },
                    { "type", type }
                }
            }
        };

        pl["params"] = nlohmann::json::object();

        if (price)
            pl["params"]["price"] = price;
        if (quantity)
            pl["params"]["quantity"] = quantity;
        if (notional)
            pl["params"]["notional"] = notional;
        if (trigger_price)
            pl["params"]["trigger_price"] = trigger_price;

        if (!client_oid.empty())
            pl["params"]["client_oid"] = client_oid;
        if (!time_in_force.empty())
            pl["params"]["time_in_force"] = time_in_force;
        if (!exec_inst.empty())
            pl["params"]["exec_inst"] = exec_inst;


        util::sign_payload(pl, this->m_key, this->m_secret);

        m_ws_client->send(pl);
    }
} // namespace crypto_com