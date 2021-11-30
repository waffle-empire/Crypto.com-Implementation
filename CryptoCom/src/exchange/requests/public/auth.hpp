#include "../base.hpp"

class AuthRequest : public BaseRequest
{
private:
    const std::string method = "public/auth";

    std::string api_key;
    long int nonce;

public:
    AuthRequest(std::string api_key)
        : BaseRequest()
    {
        this->api_key = api_key;
        this->nonce = time(nullptr) * 1000;
    }

    nlohmann::json to_json()
    {
        auto json = nlohmann::json{
            { "api_key", this->api_key },
            { "method", this->method },
            { "nonce", this->nonce },
        };
        BaseRequest::to_json(json);

        return json;
    }
};
