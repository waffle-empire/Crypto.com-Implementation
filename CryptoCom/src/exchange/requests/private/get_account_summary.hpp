#include "../base.hpp"

class GetAccountSummary : public BaseRequest
{
private:
    const std::string method = "private/get-account-summary";

    std::string currency;
    long int nonce;

public:
    GetAccountSummary(std::string currency = "")
        : BaseRequest()
    {
        this->currency = currency;
        this->nonce = time(nullptr) * 1000;
    }

    nlohmann::json to_json()
    {
        auto json = nlohmann::json{
            { "method", this->method },
            { "nonce", this->nonce },
        };

        if (this->currency.empty())
            json["params"] = nlohmann::json::object();
        else
            json["params"] = nlohmann::json{ { "currency", this->currency } };

        BaseRequest::to_json(json);

        return json;
    }
};
