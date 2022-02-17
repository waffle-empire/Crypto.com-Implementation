#include "common.hpp"
#include "crypto_com/client.hpp"
#include "util/logger.hpp"
#include "util/thread_pool.hpp"
#include "settings.hpp"

using namespace crypto_com_impl;

void signal_handler(int signum);
void subscription_handler(nlohmann::json pl);

int main()
{
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    std::signal(SIGUSR1, signal_handler);
    std::signal(SIGUSR2, signal_handler);

    std::unique_ptr<Logger> logger_instance = std::make_unique<Logger>();
    g_log->info("MAIN", "Starting app");

    // load settings
    std::unique_ptr<Settings> settings_instance = std::make_unique<Settings>();
    settings_instance->load();
    g_log->verbose("MAIN", "Initialized settings and loaded");

    std::unique_ptr<ThreadPool> thread_pool_instance = std::make_unique<ThreadPool>();
    g_log->verbose("MAIN", "Initialized thread pool.");

    // client
    std::unique_ptr<crypto_com::Client> client_instance = std::make_unique<crypto_com::Client>(g_settings->api.key, g_settings->api.secret);
    g_thread_pool->push([&client_instance]
    {
        client_instance->connect_market_socket();
    });
    
    g_thread_pool->push([&client_instance]
    {
        client_instance->connect_user_socket();
    });

    while (client_instance->user->is_authenticated() != true)
    {
        std::this_thread::sleep_for(50ms);
    }

    bool testing = true;
    nlohmann::json result;
    if (testing == true)
    {
        std::cout << "send tests" << std::endl;
        // result = client_instance->user->cancel_all_orders("CROUSDC");
        // result = client_instance->user->cancel_order("CRO_USDC", "2248896696844988229");
        // result = client_instance->user->create_order("CRO_USDC", "SELL", "LIMIT", 0.512, 51.121, 0, 0, "bot_test_02", "GOOD_TILL_CANCEL", "" );
        // result = client_instance->user->get_account_summary();
        // result = client_instance->user->get_open_orders();
        // result = client_instance->user->get_order_detail("2248896696844988229");
        // result = client_instance->user->get_order_history();
        // result = client_instance->user->get_trades("CROUSDC", 0, 0, 0, 0);
        while (!result.contains("code")){
            std::this_thread::sleep_for(100ms);
            // result = client_instance->market->subscribe_candlestick("1m", "CRO_USDC", subscription_handler);
            // OR
            result = client_instance->market->subscribe("candlestick.1m.CRO_USDC", subscription_handler);
        }
        // std::cout << result << std::endl;
    }

    // trade logic

    while (g_running)
    {
        std::this_thread::sleep_for(100ms);
    }

    client_instance->kill();
    client_instance.reset();
    g_log->verbose("MAIN", "Uninitialized crypto.com client.");

    g_log->verbose("MAIN", "Destroying thread pool.");
    thread_pool_instance->destroy();
    thread_pool_instance.reset();
    g_log->verbose("MAIN", "Uninitialized thread pool.");

    settings_instance.reset();
    g_log->verbose("MAIN", "Uninitialized settings.");

    g_log->info("MAIN", "Farewell!");
    logger_instance.reset();

    return 0;
}

void subscription_handler(nlohmann::json pl)
{
    std::cout << "subscription received" << std::endl;
    std::cout << pl << std::endl;
}

void signal_handler(int signum)
{
    g_log->info("SIGNAL", "Received termination signal, shutting down...");

    g_running = false;
}