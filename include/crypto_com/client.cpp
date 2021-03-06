#include "client.hpp"

namespace crypto_com
{
    Client::Client(std::string key, std::string secret, bool is_sandbox)
    {
        this->market    = new MarketAPI(is_sandbox);
        this->user      = new UserAPI(key, secret, is_sandbox);
    }

    Client::~Client()
    {
        delete this->market;
        delete this->user;
    }

    void Client::connect_user_socket()
    {
        this->user->connect();
    }

    void Client::connect_market_socket()
    {
        this->market->connect();
    }

    void Client::kill()
    {
        this->user->disconnect();
        this->market->disconnect();
    }
}