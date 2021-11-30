#include "nlohmann/json.hpp"

static int counter;
class BaseRequest
{
private:
    int id;
public:
    BaseRequest()
    {
        this->id = ++counter; // increment then assign
    }

    void to_json(nlohmann::json &json)
    {
        json["id"] = this->id;
    }
};
