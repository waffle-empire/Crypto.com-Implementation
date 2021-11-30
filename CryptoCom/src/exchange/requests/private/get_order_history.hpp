#include "../base.hpp"

class GetOrderHistory : public BaseRequest
{
private:
    const std::string method = "private/get-order-history";

    std::string instrument_name;
    long start_ts;
    long end_ts;
    int page_size;
    int page;
    long int nonce;

public:
    GetOrderHistory(std::string instrument_name = "", long start_ts = 0, long end_ts = 0, int page_size = 0, int page = 0)
        : BaseRequest()
    {
        this->instrument_name = instrument_name;
        this->start_ts = start_ts;
        this->end_ts = end_ts;
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

        if (this->start_ts)
            json["params"]["start_ts"] = this->start_ts;
        if (this->end_ts)
            json["params"]["end_ts"] = this->end_ts;
        if (this->page_size)
            json["params"]["page_size"] = this->page_size;
        if (this->page)
            json["params"]["page"] = this->page;

        BaseRequest::to_json(json);

        return json;
    }
};
