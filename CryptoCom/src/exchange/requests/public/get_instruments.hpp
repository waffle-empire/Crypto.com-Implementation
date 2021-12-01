#include "../base.hpp"

class GetInstruments : public BaseRequest
{
private:
    const std::string method = "public/get-instruments";


    long int nonce;

public:
    GetInstruments()
        : BaseRequest()
    {
        this->nonce = time(nullptr) * 1000;
    }

    nlohmann::json to_json()
    {
        auto json = nlohmann::json{
            { "method", this->method },
            { "nonce", this->nonce }
        };
        json["params"] = nlohmann::json::object();

        BaseRequest::to_json(json);

        return json;
    }
};
