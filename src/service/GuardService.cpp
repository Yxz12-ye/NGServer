#include "GuardService.hpp"

#include <iostream>

RepeatedTimer::RepeatedTimer(
    boost::asio::io_context& _io,
    std::chrono::milliseconds _time,
    std::function<void()> _hld) 
    :io_ctx(_io), m_timer(_io), o_timeout_handler(std::move(_hld)), repeat_time(_time)
{
    m_timeout_handler = std::move([this](const boost::system::error_code &e){
        if(this->running && !e){
            m_timer.expires_at(std::chrono::steady_clock::now()+this->repeat_time);
            m_timer.async_wait(m_timeout_handler);
            o_timeout_handler();
        }
    });
}

void RepeatedTimer::start(std::chrono::milliseconds) {
    running = true;
    m_timer.expires_at(std::chrono::steady_clock::now()+this->repeat_time);
    m_timer.async_wait(m_timeout_handler);
}

void RepeatedTimer::stop() {
    running = false;
}

GuardService::GuardService() : m_timer(m_io_ctx, m_sample_timems, [this]() { this->task(); }) {
    m_guard = std::make_unique<PlatformAbstractLayer>();
}

void GuardService::run() {
    auto work_guard = boost::asio::make_work_guard(m_io_ctx);
    m_timer.start(m_sample_timems);
    m_io_ctx.run();
}

void GuardService::task() {
    std::cerr << m_guard->GetSysInfo() << std::endl;
}
