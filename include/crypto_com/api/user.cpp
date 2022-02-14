#include "user.hpp"

namespace crypto_com
{
    UserAPI::UserAPI(std::string key, std::string secret, bool is_sandbox)
        : m_key(key), m_secret(secret), m_is_sandbox(is_sandbox)
    {
        this->m_ws_client = new WSClient(EndpointType::USER, is_sandbox);
        this->m_ws_client->set_authenticate_handler(bind(&UserAPI::authenticate_handler, this));
    }

    UserAPI::~UserAPI()
    {
        delete this->m_ws_client;
    }

    void UserAPI::connect()
    {
        this->m_ws_client->connect();
    }

    bool UserAPI::is_authenticated()
    {
        return this->m_ws_client->is_authenticated();
    }

    void UserAPI::disconnect()
    {
        this->m_ws_client->kill();
    }
}