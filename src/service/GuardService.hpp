#pragma once

#include "../utils/PlatformAbstractLayer.hpp"

#include <boost/asio.hpp>

class RepeatedTimer
{
private:
    boost::asio::io_context& io_ctx;
    boost::asio::steady_timer m_timer;
    bool running = false;
    std::atomic<bool> is_busy = false;
    boost::asio::thread_pool& o_worker;

    std::function<void()> o_timeout_handler;
    // std::function<void(const boost::system::error_code &e)> m_timeout_handler;

    void arm_next();

public:
    RepeatedTimer(boost::asio::io_context&, boost::asio::thread_pool&, std::function<void()>);
    ~RepeatedTimer() = default;

    static boost::asio::steady_timer::duration calcNextTime();

    void start(boost::asio::steady_timer::duration);
    void stop();
};

class GuardService
{
private:
    boost::asio::io_context m_io_ctx;
    std::unique_ptr<PlatformAbstractLayer> m_guard;
    std::chrono::milliseconds m_sample_timems = std::chrono::milliseconds(2000);
    boost::asio::thread_pool m_worker;
    RepeatedTimer m_timer;

public:
    GuardService();
    ~GuardService();

    void run();
    void task();
};
