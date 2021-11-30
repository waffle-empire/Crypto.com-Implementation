#include "../base.hpp"

class CreateOrder : public BaseRequest
{
private:
    const std::string method = "private/create-order";

    std::string instrument_name;
    std::string side;
    std::string type;
    double price;
    double quantity;
    double notional;
    double trigger_price;

    std::string client_oid;
    std::string time_in_force;
    std::string exec_inst;

    long int nonce;

public:
    CreateOrder(std::string instrument_name, std::string side, std::string type,
                double price = 0, double quantity = 0, double notional = 0,
                double trigger_price = 0, std::string client_oid = "",
                std::string time_in_force = "", std::string exec_inst = "")
        : BaseRequest()
    {
        this->instrument_name = instrument_name;

        this->side = side;
        this->type = type;
        this->price = price;
        this->quantity = quantity;
        this->notional = notional;
        this->trigger_price = trigger_price;

        this->client_oid = client_oid;
        this->time_in_force = time_in_force;
        this->exec_inst = exec_inst;

        this->nonce = time(nullptr) * 1000;
    }

    nlohmann::json to_json()
    {
        auto json = nlohmann::json{
            { "method", this->method },
            { "nonce", this->nonce },
            { "params", {
                    { "instrument_name", this->instrument_name },
                    { "side", this->side },
                    { "type", this->type }
                }
            }
        };

        if (this->price)
            json["params"]["price"] = this->price;
        if (this->quantity)
            json["params"]["quantity"] = this->quantity;
        if (this->notional)
            json["params"]["notional"] = this->notional;
        if (this->trigger_price)
            json["params"]["trigger_price"] = this->trigger_price;

        if (!this->client_oid.empty())
            json["params"]["client_oid"] = this->client_oid;
        if (!this->time_in_force.empty())
            json["params"]["time_in_force"] = this->time_in_force;
        if (!this->exec_inst.empty())
            json["params"]["exec_inst"] = this->exec_inst;

        BaseRequest::to_json(json);

        return json;
    }
};
