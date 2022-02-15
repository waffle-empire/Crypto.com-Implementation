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

        /* COMMON API REFERENCE - https://exchange-docs.crypto.com/spot/index.html#common-api-reference */
        /* PUBLIC */ 
        void authenticate_handler();
        // void get_instruments();
        // void get_book(std::string instrument_name, std::string depth = "");
        // void get_candlestick(std::string instrument_name = "", std::string timeframe = "");
        // void get_ticker(std::string instrument_name = ""); 
        // void get_trades(std::string instrument_name = "");  // conflicting name with private/get-trades

        /* PRIVATE */
        // void set_cancel_on_disconnect(std::string scope);
        // void get_cancel_on_disconnect();
        // void create_withdrawal(std::string currency, double amount, std::string address, 
                // std::string client_wid = "", std::string address_tag = "", std::string network_id = "");
        // void get_currency_networks(); // still need to pass empty params object in json
        // void get_withdrawal_history(std::string currency = "", double start_ts = 0, double end_ts = 0, int page_size = 0, int page = 0, std::string status = "");
        // void get_deposit_history(std::string currency = "", double start_ts = 0, double end_ts = 0, int page_size = 0, int page = 0, std::string status = "");
        // void get_deposit_address(std::string currency);
        

        // SPOT TRADING API - https://exchange-docs.crypto.com/spot/index.html#spot-trading-api
        nlohmann::json cancel_all_orders(std::string instrument_name);
        nlohmann::json cancel_order(std::string instrument_name, std::string order_id);
        nlohmann::json create_order(std::string instrument_name, std::string side, std::string type,
                double price = 0, double quantity = 0, double notional = 0,
                double trigger_price = 0, std::string client_oid = "",
                std::string time_in_force = "", std::string exec_inst = "");
        nlohmann::json get_account_summary(std::string currency = "");
        nlohmann::json get_open_orders(std::string instrument_name = "", int page_size = 0, int page = 0);
        nlohmann::json get_order_detail(std::string order_id);
        nlohmann::json get_order_history(std::string instrument_name = "", long start_ts = 0, long end_ts = 0,
                int page_size = 0, int page = 0);
        nlohmann::json get_trades(std::string instrument_name = "", long start_ts = 0, long end_ts = 0,
                int page_size = 0, int page = 0);
        
        // MARGIN TRADING API - https://exchange-docs.crypto.com/spot/index.html#margin-trading-api
        /* PUBLIC */
        
        /* PRIVATE */

        // DERIVATIVES TRANSFER API - https://exchange-docs.crypto.com/spot/index.html#derivatives-transfer-api

        // SUB-ACCOUNT API - https://exchange-docs.crypto.com/spot/index.html#sub-account-api

        void connect();
        void disconnect();
        bool is_authenticated();
    };
}