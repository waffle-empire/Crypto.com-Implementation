#include "../base.hpp"

class GetOrderDetail : public BaseRequest
{
private:
    const std::string method = "private/get-order-detail";

    std::string order_id;
    long int nonce;

public:
    GetOrderDetail(std::string order_id)
        : BaseRequest()
    {
        this->order_id = order_id;

        this->nonce = time(nullptr) * 1000;
    }

    nlohmann::json to_json()
    {
        auto json = nlohmann::json{
            { "method", this->method },
            { "nonce", this->nonce },
            {
                "params", {
                    {"order_id", this->order_id}
                }
            }
        };

        BaseRequest::to_json(json);

        return json;
    }
};
