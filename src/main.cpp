#include <iostream>
#include "service/ServiceManger.hpp"

int main(int, char**){
    boost::asio::io_context io;
    auto work_guard = boost::asio::make_work_guard(io);
    boost::asio::signal_set signals(io, SIGINT, SIGTERM);
    ServiceManger a(io);
    a.run();
    signals.async_wait([&](std::error_code, int) {
        std::cerr << "Shutting down..." << std::endl;
        a.stop();
        work_guard.reset();
    });
    io.run();
    a.join();
    return 0;
}
