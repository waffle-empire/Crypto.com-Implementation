#include "client.hpp"
#include "requests/private/get_account_summary.hpp"
#include "spot.hpp"

namespace exchange
{
    SpotTrading::SpotTrading(Client *client)
    {
        this->m_client = client;
    }

    void SpotTrading::get_account_summary(std::string currency)
    {
        auto account_summary = GetAccountSummary(currency);
        auto json = account_summary.to_json();

        this->m_client->send(json);
    }
}
