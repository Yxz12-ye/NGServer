#include "ServiceManger.hpp"

ServiceManger::ServiceManger(boost::asio::io_context& _io) : m_worker(2), o_io_ctx(_io){
    addService(std::make_unique<GuardService>(o_io_ctx, m_worker));
}

ServiceManger::~ServiceManger() {}

void ServiceManger::addService(std::unique_ptr<BaseService> _service) {
    m_services.push_back(std::move(_service));
}

void ServiceManger::run() {
    for(auto& service:m_services){
        service->start();
    }
}

void ServiceManger::stop() {
    for(auto& service:m_services){
        service->stop();
    }
}

void ServiceManger::join() {
    m_worker.join();
}
