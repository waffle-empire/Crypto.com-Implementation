#include "common.hpp"
#include "typedef.hpp"

namespace exchange
{
    class MessageHandler; // forward declaration to message_handler.hpp
    class SpotTrading;
    class Client
    {
        friend class MessageHandler;

        client *m_client;
        client::connection_ptr m_conn;

        MessageHandler *m_message_handler;

        std::string m_endpoint = "wss://stream.crypto.com/v2/user";
        std::string key;
        std::string secret;

    public:
        SpotTrading *spot;

        Client(std::string key, std::string secret);
        ~Client();

        bool connect();

        void kill();

        bool send(nlohmann::json json);
        bool send(std::string message);
        bool send(const char *message);

    private:
        bool authenticate();

        void on_message(websocketpp::connection_hdl con_hdl, message_ptr message);

        context_ptr on_tls_init();
    };
}
