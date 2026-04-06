#include "GuardService.hpp"

class ServiceManger
{
private:
    boost::asio::io_context& o_io_ctx;
    boost::asio::thread_pool m_worker;
    std::vector<std::unique_ptr<BaseService>> m_services;

public:
    ServiceManger(boost::asio::io_context&);
    ~ServiceManger();

    void addService(std::unique_ptr<BaseService>);
    void run();
    void stop();
    void join();
};
