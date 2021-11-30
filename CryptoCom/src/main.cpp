#include "common.hpp"
#include "exchange/client.hpp"
#include "thread_pool.hpp"

bool g_running = true;
void signal_handler(int signum);
int main()
{
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    std::signal(SIGUSR1, signal_handler);
    std::signal(SIGUSR2, signal_handler);

    g_log->info("MAIN", "Startup > ");

    g_log->verbose("MAIN", "Creating thread pool");
    auto thread_pool_instance = std::make_unique<thread_pool>();

    auto client_instance = std::make_unique<exchange::Client>();
    g_thread_pool->push([&]
    {
        client_instance->connect();
    });

    std::this_thread::sleep_for(2s);

    client_instance->authenticate("hpAMS5QYAf24NcVAUir8PF", "gLfT5igm8jKSJkkVCMC9Fr");

    while (g_running)
    {
        std::this_thread::sleep_for(100ms);
    }

    client_instance->kill();

    thread_pool_instance->destroy();
    thread_pool_instance.reset();

    client_instance.reset();

    return 0;
}

void signal_handler(int signum)
{
    std::cout << "\r";
    g_log->info("MAIN", "Received termination signal, exiting...");

    g_running = false;
}
