#include "../base.hpp"

class CancelOrder : public BaseRequest
{
private:
    const std::string method = "private/cancel-order";

    std::string instrument_name;
    std::string order_id;

    long int nonce;

public:
    CancelOrder(std::string instrument_name, std::string order_id)
        : BaseRequest()
    {
        this->instrument_name = instrument_name;
        this->order_id = order_id;

        this->nonce = time(nullptr) * 1000;
    }

    nlohmann::json to_json()
    {
        auto json = nlohmann::json{
            { "method", this->method },
            { "nonce", this->nonce },
            { "params", {
                    { "instrument_name", this->instrument_name },
                    { "order_id", this->order_id }
                }
            }
        };

        BaseRequest::to_json(json);

        return json;
    }
};
