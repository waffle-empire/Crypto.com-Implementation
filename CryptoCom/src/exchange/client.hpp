#include <ctime>
#include <sstream>
#include <string>
#include "cryptlite/hmac.h"
#include "cryptlite/sha256.h"
#include "websocketpp/client.hpp"
#include "websocketpp/config/asio_client.hpp"

namespace exchange
{
    void sign(nlohmann::json &json, std::string key, std::string secret)
    {
        uint8_t digest[cryptlite::sha256::HASH_SIZE];

        std::string params = "";
        if (json.find("params") != json.end())
            for (auto &item : json["params"].items())
                params += item.key() + to_string(item.value());

        std::string payload = json["method"].get<std::string>() + to_string(json["id"]) + key + params + to_string(json["nonce"]);

        cryptlite::hmac<cryptlite::sha256>::calc(payload, secret, digest);

        std::stringstream sstream;
        for (uint8_t i = 0u; i < cryptlite::sha256::HASH_SIZE; i++)
            sstream << std::hex << (int)digest[i];

        json["sig"] = sstream.str();

        std::cout << json.dump(4).c_str() << std::endl;
    }

    typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
    typedef std::shared_ptr<boost::asio::ssl::context> context_ptr;
    using websocketpp::lib::placeholders::_1;
    using websocketpp::lib::placeholders::_2;
    using websocketpp::lib::bind;

    typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

    class Client
    {
        client *m_client;
        client::connection_ptr m_conn;
        std::string m_endpoint = "wss://stream.crypto.com/v2/user";

        bool m_error = false;

    public:
        Client(std::string endpoint = "")
        {
            if (!endpoint.empty()) this->m_endpoint = endpoint;
        }

        ~Client()
        {
            delete this->m_client;
        }

        bool authenticate(std::string key, std::string secret)
        {
            nlohmann::json json{
                { "id", 11 },
                { "method", "public/auth" },
                { "api_key", key },
                { "nonce", time(nullptr) * 1000 }
            };

            sign(json, key, secret);

            websocketpp::lib::error_code ec;
            this->m_client->send(this->m_conn->get_handle(), json.dump(), websocketpp::frame::opcode::text, ec);
            if (ec)
            {
                g_log->error("CLIENT", "Authentication error:\n%s", ec.message().c_str());

                return false;
            }
            return true;
        }

        void connect()
        {
            try
            {
                this->m_client = new client();

                this->m_client->init_asio();
                this->m_client->set_tls_init_handler(bind(&Client::on_tls_init, this));

                this->m_client->set_message_handler(bind(&Client::on_message, this, _1, _2));

                websocketpp::lib::error_code ec;
                this->m_conn = this->m_client->get_connection(this->m_endpoint, ec);
                if (ec)
                {
                    this->m_error = true;

                    g_log->error("CLIENT", "Failed to get connection:\n%s", ec.message().c_str());
                }

                this->m_client->connect(this->m_conn);

                this->m_client->run();
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }

        void kill()
        {
            this->m_conn->close(1001, "");
        }

    private:
        void on_message(websocketpp::connection_hdl con_hdl, message_ptr message)
        {
            g_log->info("CLIENT", "Received msg:\n%s", message->get_payload().c_str());
        }

        context_ptr on_tls_init()
        {
            context_ptr ctx = std::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
            try {
                ctx->set_options(boost::asio::ssl::context::default_workarounds |
                                boost::asio::ssl::context::no_sslv2 |
                                boost::asio::ssl::context::no_sslv3 |
                                boost::asio::ssl::context::single_dh_use);
            } catch (std::exception &e) {
                std::cout << "Error in context pointer: " << e.what() << std::endl;
            }
            return ctx;
        }
    };
}
