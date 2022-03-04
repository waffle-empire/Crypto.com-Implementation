#pragma once
#include "../common.hpp"
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_client.hpp>

namespace crypto_com
{
    typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
    typedef std::shared_ptr<boost::asio::ssl::context> context_ptr;
    using websocketpp::lib::placeholders::_1;
    using websocketpp::lib::placeholders::_2;
    using websocketpp::lib::bind;

    typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

    enum class EndpointType
    {
        NONE,
        USER,
        MARKET
    };

    class WSClient : client
    {
        client::connection_ptr m_conn;

        bool m_authenticated = false;
        bool m_is_sandbox;
        EndpointType m_endpoint_type;

        typedef std::function<void()> auth_handler;
        auth_handler m_authenticate_handler;

        message_handler m_heartbeat_handler;

        uint64_t m_id = 0;

        std::condition_variable m_data_condition;
        std::mutex m_lock;

        std::map<uint64_t, nlohmann::json> m_message_map;
        std::map<std::string, std::function<void(nlohmann::json pl)>> m_subscription_map;

    public:
        WSClient(EndpointType endpoint_type, bool is_sandbox)
            : m_endpoint_type(endpoint_type), m_is_sandbox(is_sandbox)
        {
            this->clear_access_channels(websocketpp::log::alevel::all);

            this->init_asio();
            this->set_tls_init_handler(bind(&WSClient::on_tls_init, this));

            m_heartbeat_handler = bind(
                endpoint_type == EndpointType::USER ? &WSClient::pre_authenticate_message : &WSClient::post_authenticate_message,
                this,
                _1,
                _2
            );
            this->set_message_handler(bind(&WSClient::message_handler, this, _1, _2));
        }

        virtual ~WSClient()
        {
            
        }

        bool connect()
        {
            websocketpp::lib::error_code ec;
            this->m_conn = this->get_connection(this->get_endpoint(), ec);
            if (ec)
                return false;

            client::connect(this->m_conn);
            // blocking call
            this->run();

            return true;
        }

        std::string get_endpoint()
        {
            switch (m_endpoint_type)
            {
                case EndpointType::USER:
                    return m_is_sandbox ? "wss://uat-stream.3ona.co/v2/user" : "wss://stream.crypto.com/v2/user";
                case EndpointType::MARKET:
                    return m_is_sandbox ? "wss://uat-stream.3ona.co/v2/market" : "wss://stream.crypto.com/v2/market";
            }
            return nullptr;
        }

        void pre_authenticate_message(websocketpp::connection_hdl con_hdl, message_ptr message)
        {
            nlohmann::json pl = nlohmann::json::parse(message->get_payload());
            std::string endpoint = m_endpoint_type == EndpointType::USER ? "user" : "market";
            // std::cout << endpoint << std::endl;
            // std::cout << pl << std::endl;

            if (pl["method"] == "public/auth")
            {
                if (pl["code"] == 0)
                {
                    this->m_authenticated = true;

                    m_heartbeat_handler = bind(&WSClient::post_authenticate_message, this, _1, _2);

                    return;
                }

                std::cout << "FAILED TO AUTHENTICATE" << std::endl;

                return;
            } else if (pl["method"] == "public/heartbeat")
            {
                this->send_heartbeat(pl);

                if (m_authenticate_handler)
                    m_authenticate_handler();

                return;
            }
            return;
        }

        void post_authenticate_message(websocketpp::connection_hdl con_hdl, message_ptr message)
        {
            nlohmann::json pl;
            try {
                pl = nlohmann::json::parse(message->get_payload());
            } catch (std::exception &e) {
                std::cout << "Error in payload: " << e.what() << std::endl;
            }
            std::string endpoint = m_endpoint_type == EndpointType::USER ? "user" : "market";
            // std::cout << endpoint << std::endl;
            // std::cout << pl << std::endl;

            if (pl["method"] == "public/heartbeat")
            {
                this->send_heartbeat(pl);

                return;
            } else if (pl["method"] == "subscribe" && pl.contains("result") && pl["result"].contains("subscription")) {
                if (auto it = this->m_subscription_map.find(pl["result"]["subscription"]); it != this->m_subscription_map.end())
                {
                    std::function<void(nlohmann::json pl)> function = it->second;
                    function(pl["result"]);
                }
            } else if (pl.contains("id")) {
                std::unique_lock<std::mutex> lock(this->m_lock);

                if (auto it = this->m_message_map.find(pl["id"]); it != this->m_message_map.end())
                {
                    it->second = pl;
                }

                lock.unlock();
                this->m_data_condition.notify_all();
            }
        }

        void message_handler(websocketpp::connection_hdl con_hdl, message_ptr message)
        {
            m_heartbeat_handler(con_hdl, message);
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

        void kill()
        {
            this->m_conn->close(1001, "");
            this->stop();
        }

        bool is_authenticated()
        {
            return m_authenticated;
        }

        nlohmann::json send(nlohmann::json j)
        {
            j["id"] = ++this->m_id;
            
            this->m_message_map.emplace(j["id"], nullptr);

            if (websocketpp::lib::error_code ec = this->send_no_wait(j); ec){
                throw ec;
            }

            std::unique_lock<std::mutex> lock(this->m_lock);
            this->m_data_condition.wait(lock);

            if (auto it = this->m_message_map.find(j["id"]); it != this->m_message_map.end())
            {
                return it->second;
            }
            return nullptr;
        }

        websocketpp::lib::error_code send_no_wait(nlohmann::json j)
        {
            websocketpp::lib::error_code ec;
            std::string message = j.dump();

            client::send(this->m_conn->get_handle(), message.c_str(), websocketpp::frame::opcode::text, ec);
            return ec;
        }

        nlohmann::json add_subscription(std::string channel, std::function<void(nlohmann::json pl)> function)
        {            
            nlohmann::json pl = nlohmann::json{
                { "method", "subscribe" },
                { "nonce", util::create_nonce() },
                { "params", { 
                        { "channels", nlohmann::json::array({ channel }) }
                    }
                }
            };

            this->m_subscription_map.emplace(pl["params"]["channels"][0], function);

            return this->send(pl);
        }

        void send_heartbeat(nlohmann::json& pl)
        {
            this->send_no_wait({
                { "method", "public/respond-heartbeat" },
                { "id", pl["id"] }
            });
        }

        void set_authenticate_handler(auth_handler f)
        {
            m_authenticate_handler = f;
        }
    };
}