#include "aries_brpc.h"
#include "aries_log.h"

namespace AriesBrpc
{
    void Channels::insert(const std::string &addr)
    {
        std::unique_lock<std::mutex> lock(_mutex);

        auto channel = std::make_shared<brpc::Channel>();
        brpc::ChannelOptions option;
        option.protocol = "baidu_std";
        channel->Init(addr.c_str(), &option);
        _channels.push_back(channel);
        _channel_map.insert(std::make_pair(addr, channel));
    }

    void Channels::remove(const std::string &addr)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        auto it = _channel_map.find(addr);
        if (it == _channel_map.end())
        {
            WARNING("channel not found: %s", addr.c_str());
        }
        _channel_map.erase(it);
        for (auto iter = _channels.begin(); iter != _channels.end();++iter)
        {
            if (*iter == it->second)
            {
                iter = _channels.erase(iter);
                break;
            }
        }
    }
    ChannelPtr Channels::get()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _idx = (_idx + 1) % _channels.size();
        return _channels[_idx];
    }

    Channels::ptr ServiceChannels::find(const std::string &service_name)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        auto it = _service_channels_map.find(service_name);
        if (it == _service_channels_map.end())
        {
            WARNING("service not found: %s", service_name.c_str());
            return Channels::ptr();
        }
        return it->second;
    }

    void ServiceChannels::set_watch(const std::string &service_name)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        auto channels = std::make_shared<Channels>(); // 根据服务名称创建channel集合
        _service_channels_map.insert(std::make_pair(service_name, channels)); // 该service_name就对应该 channel集合
    }

    void ServiceChannels::insert(const std::string &service_name, const std::string &addr)
    {
        Channels::ptr channels = find(service_name);// 查找channel集合
        if(!channels) {
            return;
        }
        channels->insert(addr);
    }
    void ServiceChannels::remove(const std::string &service_name, const std::string &addr)
    {
        Channels::ptr channels = find(service_name);// 查找channel集合
        if(!channels) {
            return;
        }
        channels->remove(addr);
    }
    ChannelPtr ServiceChannels::get(const std::string &service_name)
    {
        Channels::ptr channels = find(service_name);// 查找channel集合
        if(!channels) {
            return ChannelPtr();
        }
        return channels->get();
    }

    google::protobuf::Closure* ClosureFactory::create(callback_t &&cb) {
        auto obj = std::make_shared<ClosureFactory::Object>();
        obj->cb = std::move(cb);
        return brpc::NewCallback(&ClosureFactory::async_callback, obj);
    }
    void ClosureFactory::async_callback(const ClosureFactory::Object::ptr obj)
    {
        obj->cb();
    }

    std::shared_ptr<brpc::Server> RpcServerFactory::create(int port, google::protobuf::Service *service) {
        brpc::ServerOptions options;
        options.idle_timeout_sec = -1;
        auto server = std::make_shared<brpc::Server>();
        int ret = server->AddService(service, brpc::SERVER_OWNS_SERVICE);
        if(ret != 0) {
            ERROR("AddService failed: %d", ret);
            abort();
        }
        ret = server->Start(port, &options);
        if (ret != 0)
        {
            ERROR("Start failed: %d", ret);
            abort();
        }
        return server;
    }
}
