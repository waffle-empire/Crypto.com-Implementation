#include "client.hpp"
#include "requests/private/cancel_all_orders.hpp"
#include "requests/private/get_account_summary.hpp"
#include "spot.hpp"

namespace exchange
{
    SpotTrading::SpotTrading(Client *client)
    {
        this->m_client = client;
    }

    void SpotTrading::cancel_all_orders(std::string instrument_name)
    {
        auto cancel_all_orders = CancelAllOrders(instrument_name);
        auto json = cancel_all_orders.to_json();

        this->m_client->send(json);
    }

    void SpotTrading::get_account_summary(std::string currency)
    {
        auto account_summary = GetAccountSummary(currency);
        auto json = account_summary.to_json();

        this->m_client->send(json);
    }
}
