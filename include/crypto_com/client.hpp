#pragma once
#include "common.hpp"
#include "api/market.hpp"
#include "api/user.hpp"

namespace crypto_com
{
    class Client
    {
    private:
        std::string m_key;
        std::string m_secret;
    
    public:
        MarketAPI* market;
        UserAPI* user;

        Client(std::string key, std::string secret, bool is_sandbox = false);
        virtual ~Client();

        void connect_user_socket();
        void kill();
    };
} // namespace crypto_com