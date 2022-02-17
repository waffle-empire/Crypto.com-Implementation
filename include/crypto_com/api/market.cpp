#include "market.hpp"

namespace crypto_com
{
    MarketAPI::MarketAPI(bool is_sandbox)
        : m_is_sandbox(is_sandbox)
    {
        this->m_ws_client = new WSClient(EndpointType::MARKET, is_sandbox);
    }

    MarketAPI::~MarketAPI()
    {
        
    }

    void MarketAPI::connect()
    {
        this->m_ws_client->connect();
    }

    void MarketAPI::disconnect()
    {
        this->m_ws_client->kill();
    }

    bool MarketAPI::is_authenticated()
    {
        return this->m_ws_client->is_authenticated();
    }

    nlohmann::json MarketAPI::subscribe_candlestick(std::string timeframe, std::string coin_pair, std::function<void(nlohmann::json pl)> function)
    {
        std::string channel = "candlestick."+timeframe+"."+coin_pair;
        return this->m_ws_client->add_subscription(channel, function);
    }

    nlohmann::json MarketAPI::subscribe(std::string channel, std::function<void(nlohmann::json pl)> function)
    {
        return this->m_ws_client->add_subscription(channel, function);
    }


}