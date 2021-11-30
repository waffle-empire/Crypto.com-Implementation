#include "../base.hpp"

class CancelAllOrders : public BaseRequest
{
private:
    const std::string method = "private/cancel-all-orders";

    std::string instrument_name;
    long int nonce;

public:
    CancelAllOrders(std::string instrument_name)
        : BaseRequest()
    {
        this->instrument_name = instrument_name;
        this->nonce = time(nullptr) * 1000;
    }

    nlohmann::json to_json()
    {
        auto json = nlohmann::json{
            { "method", this->method },
            { "nonce", this->nonce },
            { "params", {
                    { "instrument_name", this->instrument_name }
                }
            }
        };

        BaseRequest::to_json(json);

        return json;
    }
};
