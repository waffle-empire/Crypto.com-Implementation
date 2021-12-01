namespace exchange
{
    class Client;
    class CommonApiReference
    {
        Client *m_client;

    public:
        CommonApiReference(Client *client);

        void get_instruments();
        void create_withdrawal(std::string currency, long amount, std::string address, std::string address_tag = "", std::string client_wid = "");
        void get_withdrawal_history(std::string currency = "", long start_ts = 0, long end_ts = 0, int page_size = 0, int page = 0, std::string status = "");

        /// WEBSOCKET ONLY ENDPOINTS

        void set_cancel_on_disconnect(std::string scope = "");
        void get_cancel_on_disconnect();

        /// REST ONLY ENDPOINTS

        void get_book(std::string instrument_name, std::string depth = "");
        void get_candlestick(std::string instrument_name, std::string timeframe = "");
        void get_ticker(std::string instrument_name = "");
        void get_trades(std::string instrument_name = "");
        void get_deposit_history(std::string currency = "", long start_ts = 0, long end_ts = 0, int page_size = 0, int page = 0, std::string status = "");
        void get_deposit_address(std::string currency);

    };
}
