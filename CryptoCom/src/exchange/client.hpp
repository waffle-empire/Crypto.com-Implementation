#include "common.hpp"
#include "typedef.hpp"

namespace exchange
{
    class MessageHandler; // forward declaration to message_handler.hpp
    class Client
    {
        friend class MessageHandler;

        client *m_client;
        client::connection_ptr m_conn;

        MessageHandler *m_message_handler;

        std::string m_endpoint = "wss://stream.crypto.com/v2/user";

        bool m_error = false;

    public:
        Client(std::string endpoint = "");
        ~Client();

        bool authenticate(std::string key, std::string secret);

        void connect();

        void kill();

        bool send(nlohmann::json json);
        bool send(std::string message);
        bool send(const char *message);

    private:
        void on_message(websocketpp::connection_hdl con_hdl, message_ptr message);

        context_ptr on_tls_init();
    };
}
