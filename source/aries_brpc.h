#include <brpc/channel.h>
#include <brpc/server.h>
#include <mutex>
#include <memory>
#include <vector>
#include <unordered_map>

namespace AriesBrpc
{
    using ChannelPtr = std::shared_ptr<brpc::Channel>;
    // channel集合（一个服务可能有多个主机来提供）
    class Channels
    {
    public:
        using ptr = std::shared_ptr<Channels>;

        Channels();
        void insert(const std::string &addr);
        void remove(const std::string &addr);
        ChannelPtr get() ;

    public:
        std::mutex _mutex;
        std::string _service_name;                                // 服务名称
        uint32_t _idx;                                            // 轮询下标
        std::vector<ChannelPtr> _channels;                        // 方便寻找
        std::unordered_map<std::string, ChannelPtr> _channel_map; // 便于增删
    };

    // 管理channel集合
    class ServiceChannels
    {
    public:
        ServiceChannels();
        void set_watch(const std::string &service_name);
        void insert(const std::string &service_name, const std::string &addr);
        void remove(const std::string &service_name, const std::string &addr);
        ChannelPtr get(const std::string &service_name);
    private:
        Channels::ptr find(const std::string &service_name);
    public:
        std::mutex _mutex;
        std::unordered_map<std::string, Channels::ptr> _service_channels_map;
    };

    // 异步closure
    class ClosureFactory
    {
    public:
        using callback_t = std::function<void()>;
        static google::protobuf::Closure *create(callback_t &&cb);

    public:
        struct Object
        {
            using ptr = std::shared_ptr<Object>;
            callback_t cb;
        };
        static void async_callback(const Object::ptr obj);
    };

    class RpcServerFactory
    {
    public:
        static std::shared_ptr<brpc::Server> create(int port, google::protobuf::Service *service);
    };
}
