#include "websocketpp/client.hpp"
#include "websocketpp/config/asio_client.hpp"

namespace exchange
{
    typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
    typedef std::shared_ptr<boost::asio::ssl::context> context_ptr;
    using websocketpp::lib::placeholders::_1;
    using websocketpp::lib::placeholders::_2;
    using websocketpp::lib::bind;

    typedef websocketpp::config::asio_client::message_type::ptr message_ptr;
}
