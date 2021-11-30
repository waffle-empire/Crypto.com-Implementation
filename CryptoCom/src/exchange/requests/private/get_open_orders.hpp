#include "../base.hpp"

class GetOpenOrders : public BaseRequest
{
private:
    const std::string method = "private/get-open-orders";

    std::string instrument_name;
    int page_size;
    int page;

    long int nonce;

public:
    GetOpenOrders(std::string instrument_name = "", int page_size = 0, int page = 0)
        : BaseRequest()
    {
        this->instrument_name = instrument_name;
        this->page_size = page_size;
        this->page = page;

        this->nonce = time(nullptr) * 1000;
    }

    nlohmann::json to_json()
    {
        auto json = nlohmann::json{
            { "method", this->method },
            { "nonce", this->nonce }
        };

        json["params"] = nlohmann::json::object();

        if (!this->instrument_name.empty())
            json["params"]["instrument_name"] = this->instrument_name;
        if (this->page_size)
            json["params"]["page_size"] = this->page_size;
        if (this->page)
            json["params"]["page"] = this->page;

        BaseRequest::to_json(json);

        return json;
    }
};
