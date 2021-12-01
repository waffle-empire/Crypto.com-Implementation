#include "client.hpp"
#include "requests/public/get_instruments.hpp"
#include "common_api_reference.hpp"

namespace exchange
{
    CommonApiReference::CommonApiReference(Client *client)
    {
        this->m_client = client;
    }

    void CommonApiReference::get_instruments()
    {
        auto get_instruments = GetInstruments();
        auto json = get_instruments.to_json();

        this->m_client->send(json);
    }
}
