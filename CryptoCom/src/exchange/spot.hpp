namespace exchange
{
    class Client;
    class SpotTrading
    {
        Client *m_client;

    public:
        SpotTrading(Client *client);

        void cancel_all_orders(std::string instrument_name);
        void cancel_order(std::string instrument_name, std::string order_id);
        void create_order(std::string instrument_name, std::string side, std::string type,
            double price = 0, double quantity = 0, double notional = 0,
            double trigger_price = 0, std::string client_oid = "",
            std::string time_in_force = "", std::string exec_inst = "");
        void get_account_summary(std::string currency = "");
        void get_open_orders(std::string instrument_name, int page_size, int page);
        void get_order_detail(std::string order_id);
        void get_order_history(std::string instrument_name, long start_ts, long end_ts, int page_size, int page);
        void get_trades(std::string instrument_name, long start_ts, long end_ts, int page_size, int page);
    };
}
