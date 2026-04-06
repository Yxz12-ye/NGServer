#include "GuardService.hpp"

#include <iostream>
#include <ctime>

boost::asio::steady_timer::duration RepeatedTimer::calcNextTime() {
        using namespace std::chrono;
    auto now_sys = system_clock::now();
    auto now_tt  = system_clock::to_time_t(now_sys);

    std::tm tm_now{};
#ifdef _WIN32
    localtime_s(&tm_now, &now_tt);
#else
    localtime_r(&now_tt, &tm_now);
#endif

    int min = tm_now.tm_min;
    int sec = tm_now.tm_sec;

    int add_min = 5 - (min % 5);
    if (add_min == 5 && sec == 0) {
        // 恰好在边界（如 12:10:00），下一次应该是 5 分钟后，不是立刻触发
        add_min = 5;
    }

    tm_now.tm_min += add_min;
    tm_now.tm_sec = 0;

    auto next_tt  = std::mktime(&tm_now);
    auto next_sys = system_clock::from_time_t(next_tt);

    auto d = duration_cast<steady_clock::duration>(next_sys - now_sys);

    if (d <= steady_clock::duration::zero()) {
        d = seconds(1);
    }
    return d;
}

void RepeatedTimer::arm_next() {
    m_timer.expires_after(RepeatedTimer::calcNextTime());
    m_timer.async_wait([this](const boost::system::error_code& e){
        if(!running||e==boost::asio::error::operation_aborted) return;
        if (e){
            arm_next();
            return;
        }
        arm_next();

        bool expected = false;
        if(!is_busy.compare_exchange_strong(expected,true)){
            std::cerr << "The task took longer than expected" << std::endl;
            return;
        }

        boost::asio::post(o_worker, [this](){
            o_timeout_handler(); // 约1秒
            is_busy.store(false);
        });
    });
}

RepeatedTimer::RepeatedTimer(boost::asio::io_context& _io, boost::asio::thread_pool& _worker,
                             std::function<void()> _hld)
    : io_ctx(_io), m_timer(_io), o_timeout_handler(std::move(_hld)), o_worker(_worker) {
    // m_timeout_handler = std::move([this](const boost::system::error_code &e){
    //     if(this->running && !e){
    //         m_timer.expires_after(RepeatedTimer::calcNextTime());
    //         m_timer.async_wait(m_timeout_handler);
    //         o_timeout_handler();
    //     }
    // });
}

void RepeatedTimer::start(boost::asio::steady_timer::duration nextTime) {
    running = true;
    arm_next();
}

void RepeatedTimer::stop() {
    running = false;
    m_timer.cancel();
}

GuardService::GuardService(boost::asio::io_context& _io, boost::asio::thread_pool& _pool) 
: BaseService(_io, _pool), m_timer(o_io_ctx, m_worker, [this]() { this->task(); }), m_worker(1) {
    m_guard = std::make_unique<PlatformAbstractLayer>();
}

GuardService::~GuardService() {

}

void GuardService::start() {
    // auto work_guard = boost::asio::make_work_guard(o_io_ctx);
    // m_timer.start(RepeatedTimer::calcNextTime());
    m_timer.start(m_sample_timems);
    running_.store(true);
}

void GuardService::stop() {
    running_.store(false);
    m_timer.stop();
}

void GuardService::join() {}

void GuardService::task() {
    // 这里自行判断拿到的数据是否有效, 以及数据库问题...
    std::cerr << m_guard->GetSysInfo() << std::endl;
}
