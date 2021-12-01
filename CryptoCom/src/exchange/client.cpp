#include "client.hpp"
#include "message_handler.hpp"
#include "requests/public/auth.hpp"
#include "spot.hpp"
#include "thread_pool.hpp"
#include "util.hpp"

namespace exchange
{
// public
    Client::Client(std::string key, std::string secret)
    {
        this->key = key;
        this->secret = secret;

        this->spot = new SpotTrading(this);

        this->m_message_handler = new MessageHandler(this);
    }

    Client::~Client()
    {
        delete this->m_client;
        delete this->m_message_handler;
    }

    bool Client::authenticate()
    {
        auto auth = AuthRequest(this->key);
        auto json = auth.to_json();

        util::sign(json, this->key, this->secret);

        return this->send(json);
    }

    bool Client::connect()
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
                g_log->error("CLIENT", "Failed to get connection:\n%s", ec.message().c_str());

                return false;
            }

            this->m_client->connect(this->m_conn);

            g_thread_pool->push([&]
            {
                std::this_thread::sleep_for(2s);

                this->authenticate();

                std::this_thread::sleep_for(500ms);

                this->spot->get_trades();
            });

            this->m_client->run();
        }
        catch(const std::exception& e)
        {
            g_log->error("CLIENT", "Failed to initialze client:\n%s", e.what());

            return false;
        }
        return true;
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
            g_log->error("CLIENT", "Failed to send message:\n%s", ec.message().c_str());

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
