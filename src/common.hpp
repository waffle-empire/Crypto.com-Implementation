#pragma once

#include <atomic>
#include <condition_variable>
#include <fstream>
#include <functional>
#include <memory>
#include <stack>
#include <thread>

#include <csignal>

#include <nlohmann/json.hpp>

namespace crypto_com_impl
{
    inline std::atomic<bool> g_running{ true };

    using namespace std::literals::chrono_literals;
} // namespace crypto_com_impl
