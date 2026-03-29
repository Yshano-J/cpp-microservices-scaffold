#include "aries_etcd_service_register.h"

namespace AriesService
{
    void wait_for_connection(etcd::Client &client)
    {
        while (!client.head().get().is_ok())
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    SvcProvider::SvcProvider(const std::string &reg_center_addr, const std::string &service_name, const std::string &service_addr) 
        : _reg_center_addr(reg_center_addr) 
        , _instance_id(aries_util::RANDOM::generateRandomString(32))
        , _service_name(service_name)
        , _service_addr(service_addr)
        , _keep_alive(nullptr) {}

    std::string SvcProvider::make_key() {
        return "/" + _service_name + "/" + _instance_id;
    }

    void SvcProvider::svcRegister()
    {
        // 实例化etcd客户端对象
        etcd::Client client(_reg_center_addr);
        wait_for_connection(client);
        // 创建租约
        etcd::Response lease = client.leasegrant(3).get();
        if (!lease.is_ok())
        {
            ERROR("leasegrant failed");
            return;
        }
        // 获取租约ID
        auto lease_id = lease.value().lease();
        auto response = client.put(make_key(), _service_addr, lease_id).get();

        if (!response.is_ok())
        {
            ERROR("put failed");
            return;
        }

        // 保活处理函数
        auto handler = [this](std::exception_ptr ep)
        {
            this->svcRegister();
        };
        // 保活
        _keep_alive.reset(new etcd::KeepAlive(_reg_center_addr, handler, 3, lease_id));
    }


    SvcWatcher::SvcWatcher(const std::string &reg_center_addr, 
        SvcWatcher::ModifyCallback &&online_callback, 
        SvcWatcher::ModifyCallback &&offline_callback) 
        : _reg_center_addr(reg_center_addr)
        , _online_callback(online_callback)
        , _offline_callback(offline_callback)
    {}

    std::string parse_key(const std::string &key)
    {  
        std::vector<std::string> parts;
        aries_util::STRING::split(key, "/", parts);
        return parts[0];
    }

    bool SvcWatcher::watch()
    {
        // 实例化etcd客户端对象
        etcd::Client client(_reg_center_addr);
        wait_for_connection(client);
        auto response = client.ls("/").get(); // ls 浏览目录
        if (response.is_ok())
        {
            auto values = response.values();
            for (auto &value : values)
            {
                std::string service_name = parse_key(value.key());
                std::string service_addr = value.as_string();
                if (_online_callback)
                    _online_callback(service_name, service_addr);
            }
        }
        // 监控
        auto callback = [this](etcd::Response resp) -> void
        {
            if (!resp.is_ok())
            {
                ERROR("watch failed");
            }
            const auto &events = resp.events();
            for (auto &event : events)
            {
                if (event.event_type() == etcd::Event::EventType::PUT) {
                    std::string service_name = parse_key(event.kv().key());
                    std::string service_addr = event.kv().as_string();
                    if (_online_callback)
                        _online_callback(service_name, service_addr);
                    INFO("service {} is online, addr: {}", service_name, service_addr);
                }
                else if (event.event_type() == etcd::Event::EventType::DELETE_) {
                    std::string service_name = parse_key(event.prev_kv().key()); // prev_kv 是删除前的key值，用于获取服务名
                    std::string service_addr = event.prev_kv().as_string(); // prev_kv 是删除前的value值，用于获取服务地址
                    if (_offline_callback)
                        _offline_callback(service_name, service_addr);
                    INFO("service {} is offline, addr: {}", service_name, service_addr);
                }
                else {
                    WARNING("unknown event type: " + std::to_string(static_cast<int>(event.event_type())));
                }
            }
        };

        _watcher.reset(new etcd::Watcher(client, "/", callback, true));
        _watcher->Wait([this](bool cancelled) {
            if (cancelled)
            {
                INFO("watcher's reconnect loop stopped as been cancelled");
                return;
            }
            watch(); // 重新浏览目录变化
        });
        return true;
    }
}
