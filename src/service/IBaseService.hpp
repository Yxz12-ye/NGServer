#include <functional>
#include <atomic>
#include <string>
#include <boost/asio.hpp>
#include <mutex>

class BaseService {
public:
    BaseService(boost::asio::io_context& _io, boost::asio::thread_pool& _worker):o_io_ctx(_io),o_worker(_worker){}
    virtual ~BaseService() = default;

    virtual std::string name() const = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void join() = 0;
    virtual bool is_running() const { return running_; }
    
protected:
    std::atomic<bool> running_{false};
    boost::asio::io_context& o_io_ctx;
    boost::asio::thread_pool& o_worker;
};