#include "etcd/Client.hpp"
#include "etcd/Response.hpp"
#include "etcd/KeepAlive.hpp"
#include "etcd/Value.hpp"
#include "etcd/Watcher.hpp"
#include <iostream>

int main()
{
    const std::string url = "http://192.168.237.129:2379";
    etcd::Client client(url);
    auto response = client.get("password").get();
    if (!response.is_ok())
    {
        std::cout << "get failed" << std::endl;
        return 1;
    }

    // 监控
    etcd::Watcher watcher(url, "password", 
        [](etcd::Response resp) -> void {
            if(!resp.is_ok()) {
                std::cout << "get failed" << std::endl;
                return;
            }
            const auto& events = resp.events();

            for (auto &event : events) {
                if(event.event_type() == etcd::Event::EventType::PUT) {
                    std::cout << "数据改变: " << event.prev_kv().as_string() 
                             << " -> " << event.kv().as_string() << std::endl;
                } else if(event.event_type() == etcd::Event::EventType::DELETE_) {
                    std::cout << "数据删除: " << event.kv().as_string() << std::endl;
                } else {
                    std::cout << "未知事件类型: " << static_cast<int>(event.event_type()) << std::endl;
                }
            } 
        }, false);

    watcher.Wait();

    return 0;
}
