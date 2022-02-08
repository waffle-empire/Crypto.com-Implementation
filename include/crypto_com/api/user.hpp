#pragma once
#include "../common.hpp"
#include "../ws/ws_client.hpp"

namespace crypto_com
{
    class UserAPI
    {
    private:
        std::string m_key;
        std::string m_secret;
        bool m_is_sandbox;

        WSClient* m_ws_client;
    public:
        UserAPI(std::string key, std::string secret, bool is_sandbox);
        virtual ~UserAPI();

        void authenticate_handler();

        void connect();
        void disconnect();
    };
} // namespace crypto_com
