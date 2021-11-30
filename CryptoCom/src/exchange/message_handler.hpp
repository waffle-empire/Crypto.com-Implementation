#include "typedef.hpp"

namespace exchange
{
    class Client; // forward declaration to client.hpp
    class MessageHandler
    {
    private:
        Client *m_client;
    public:
        MessageHandler(Client *client)
        {
            this->m_client = client;
        }
        ~MessageHandler()
        {
            this->m_client = nullptr;
        }

        void push(message_ptr message)
        {
            nlohmann::json j = nlohmann::json::parse(message->get_payload());

            if (j["method"] == "public/heartbeat")
            {
                this->handle_heartbeat(j["id"]);

                return;
            }
            g_log->verbose("MESSAGE_HANDLER", "Received message:\n%s", j.dump(4).c_str());
        }

    private:
        void handle_heartbeat(int id)
        {
            this->m_client->send(nlohmann::json{
                { "method", "public/respond-heartbeat" },
                { "id", id }
            });

            g_log->verbose("MESSAGE_HANDLER", "Handled heartbeat");
        }
    };
}
