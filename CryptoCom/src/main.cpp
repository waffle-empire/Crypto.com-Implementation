#include "common.hpp"
#include "exchange/client.hpp"
#include "settings.hpp"
#include "thread_pool.hpp"

bool g_running = true;
void signal_handler(int signum);
int main()
{
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    std::signal(SIGUSR1, signal_handler);
    std::signal(SIGUSR2, signal_handler);

    g_log->info("MAIN", "Startup");

    g_log->verbose("MAIN", "Creating settings instance");
    auto settings_instance = std::make_unique<settings>();
    settings_instance->load();

    g_log->verbose("MAIN", "Creating thread pool instance");
    auto thread_pool_instance = std::make_unique<thread_pool>();

    g_log->verbose("MAIN", "Creating Exchange Client instance");
    auto client_instance = std::make_unique<exchange::Client>();
    g_thread_pool->push([&]
    {
        client_instance->connect();
    });

    std::this_thread::sleep_for(5s);

    client_instance->authenticate(settings_instance->api.key, settings_instance->api.secret);

    while (g_running)
    {
        std::this_thread::sleep_for(100ms);
    }

    g_log->verbose("MAIN", "Killing client before destroying thread pool.");
    client_instance->kill();

    g_log->verbose("MAIN", "Destroying thread pool.");
    thread_pool_instance->destroy();

    g_log->verbose("MAIN", "Releasing client instance.");
    client_instance.reset();
    g_log->verbose("MAIN", "Releasing thread pool instance.");
    thread_pool_instance.reset();
    g_log->verbose("MAIN", "Releasing settings instance.");
    settings_instance.reset();

    return 0;
}

void signal_handler(int signum)
{
    std::cout << "\r";
    g_log->info("MAIN", "Received termination signal, exiting...");

    g_running = false;
}
