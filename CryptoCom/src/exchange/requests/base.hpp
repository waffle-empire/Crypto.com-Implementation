#include "nlohmann/json.hpp"

#ifndef BASE_REQUEST
#define BASE_REQUEST

inline int counter;
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

#endif
