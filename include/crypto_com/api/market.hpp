#pragma once
#include "../common.hpp"
#include "../ws/ws_client.hpp"

namespace crypto_com
{
    class MarketAPI
    {
    private:
        bool m_is_sandbox;

        WSClient* m_ws_client;
    public:
        MarketAPI(bool is_sandbox);
        ~MarketAPI();

        nlohmann::json subscribe_candlestick(std::string timeframe, std::string coin_pair, std::function<void(nlohmann::json pl)> function);
        nlohmann::json MarketAPI::subscribe(std::string channel, std::function<void(nlohmann::json pl)> function);

        void connect();
        void disconnect();
        bool is_authenticated();

    };
} // namespace crypto_com
