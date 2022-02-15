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

    void MarketAPI::subscribe_candlestick(std::string timeframe, std::string coin_pair, std::function<void(nlohmann::json pl)> function)
    {
        this->m_ws_client->add_subscription(timeframe, coin_pair, function);
    }
}