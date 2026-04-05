#pragma once

#include "../utils/PlatformAbstractLayer.hpp"

#include <boost/asio.hpp>

class RepeatedTimer
{
private:
    boost::asio::io_context& io_ctx;
    boost::asio::steady_timer m_timer;
    bool running = false;
    std::chrono::milliseconds repeat_time;

    std::function<void()> o_timeout_handler;
    std::function<void(const boost::system::error_code &e)> m_timeout_handler;

public:
    RepeatedTimer(boost::asio::io_context&, std::chrono::milliseconds, std::function<void()>);
    ~RepeatedTimer() = default;

    void start(std::chrono::milliseconds);
    void stop();
};

class GuardService
{
private:
    boost::asio::io_context m_io_ctx;
    std::unique_ptr<PlatformAbstractLayer> m_guard;
    std::chrono::milliseconds m_sample_timems = std::chrono::milliseconds(2000);
    RepeatedTimer m_timer;

public:
    GuardService();
    ~GuardService() = default;

    void run();
    void task();
};
