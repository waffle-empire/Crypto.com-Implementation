#include "client.hpp"
#include "requests/private/cancel_all_orders.hpp"
#include "requests/private/cancel_order.hpp"
#include "requests/private/create_order.hpp"
#include "requests/private/get_account_summary.hpp"
#include "requests/private/get_order_detail.hpp"
#include "requests/private/get_order_history.hpp"
#include "requests/private/get_open_orders.hpp"
#include "requests/private/get_trades.hpp"
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

    void SpotTrading::cancel_order(std::string instrument_name, std::string order_id)
    {
        auto cancel_order = CancelOrder(
            instrument_name, order_id
        );
        auto json = cancel_order.to_json();

        this->m_client->send(json);
    }

    void SpotTrading::create_order(std::string instrument_name, std::string side, std::string type,
        double price, double quantity, double notional,
        double trigger_price, std::string client_oid,
        std::string time_in_force, std::string exec_inst)
    {
        auto create_order = CreateOrder(
            instrument_name, side, type, price,
            quantity, notional, trigger_price,
            client_oid, time_in_force
        );
        auto json = create_order.to_json();

        this->m_client->send(json);
    }

    void SpotTrading::get_account_summary(std::string currency)
    {
        auto account_summary = GetAccountSummary(currency);
        auto json = account_summary.to_json();

        this->m_client->send(json);
    }

    void SpotTrading::get_order_detail(std::string order_id)
    {
        auto get_order_detail = GetOrderDetail(
            order_id
        );
        auto json = get_order_detail.to_json();

        this->m_client->send(json);
    }

    void SpotTrading::get_order_history(std::string instrument_name, long start_ts, long end_ts, int page_size, int page)
    {
        auto get_order_history = GetOrderHistory(
            instrument_name, start_ts, end_ts, page_size, page
        );
        auto json = get_order_history.to_json();

        this->m_client->send(json);
    }

    void SpotTrading::get_open_orders(std::string instrument_name, int page_size, int page)
    {
        auto get_open_orders = GetOpenOrders(
            instrument_name, page_size, page
        );
        auto json = get_open_orders.to_json();

        this->m_client->send(json);
    }

    void SpotTrading::get_trades(std::string instrument_name, long start_ts, long end_ts, int page_size, int page)
    {
        auto get_trades = GetTrades(
            instrument_name, start_ts, end_ts, page_size, page
        );
        auto json = get_trades.to_json();

        this->m_client->send(json);
    }
}
