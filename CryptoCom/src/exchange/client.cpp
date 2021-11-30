#include "client.hpp"
#include "message_handler.hpp"
#include "util.hpp"

namespace exchange
{
// public
    Client::Client(std::string endpoint)
    {
        if (!endpoint.empty()) this->m_endpoint = endpoint;

        m_message_handler = new MessageHandler(this);
    }

    Client::~Client()
    {
        delete this->m_client;
        delete this->m_message_handler;
    }

    bool Client::authenticate(std::string key, std::string secret)
    {
        nlohmann::json json{
            { "id", 11 },
            { "method", "public/auth" },
            { "api_key", key },
            { "nonce", time(nullptr) * 1000 }
        };

        util::sign(json, key, secret);

        websocketpp::lib::error_code ec;
        this->m_client->send(this->m_conn->get_handle(), json.dump(), websocketpp::frame::opcode::text, ec);
        if (ec)
        {
            g_log->error("CLIENT", "Authentication error:\n%s", ec.message().c_str());

            return false;
        }
        return true;
    }

    void Client::connect()
    {
        try
        {
            this->m_client = new client();
            this->m_client->clear_access_channels(websocketpp::log::alevel::all);

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
            g_log->error("CLIENT", "Failed to initialze client:\n%s", e.what());
        }
    }

    void Client::kill()
    {
        this->m_conn->close(1001, "");
    }

    bool Client::send(nlohmann::json j) { return this->send(j.dump().c_str()); }
    bool Client::send(std::string m) { return this->send(m.c_str()); }
    bool Client::send(const char *message)
    {
        websocketpp::lib::error_code ec;
        this->m_client->send(this->m_conn->get_handle(), message, websocketpp::frame::opcode::text, ec);

        if (ec)
        {
            g_log->verbose("CLIENT", "Failed to send message:\n%s", ec.message());

            return false;
        }
        return true;
    }

// private
    void Client::on_message(websocketpp::connection_hdl con_hdl, message_ptr message)
    {
        this->m_message_handler->push(message);
    }

    context_ptr Client::on_tls_init()
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
}
