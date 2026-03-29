#include "etcd/Client.hpp"
#include "etcd/Response.hpp"
#include "etcd/KeepAlive.hpp"
#include "etcd/Value.hpp"
#include "etcd/Watcher.hpp"
#include <iostream>

void get(const std::shared_ptr<etcd::Client>& client, std::shared_ptr<etcd::Watcher>& watcher, const std::string& key, const std::string &url)
{
    auto response = client->ls(key).get(); // ls 浏览目录
    if (response.is_ok())
    {
        auto values = response.values();
        for (auto &value : values)
        {
            std::cout << "key: " << value.key() << ", value: " << value.as_string() << std::endl;
        }
    }
    // 监控
    auto callback =
        [](etcd::Response resp) -> void {
            if(!resp.is_ok())
            {
                std::cout << "watch failed" << std::endl;
            }
            const auto& events = resp.events();
            for (auto &event : events) {
                if(event.event_type() == etcd::Event::EventType::PUT)
                    std::cout << "数据改变: " << event.prev_kv().as_string() << " -> " << event.kv().as_string() << std::endl;
                else if (event.event_type() == etcd::Event::EventType::DELETE_)
                    std::cout << "数据删除: " << event.kv().as_string() << std::endl;
                else
                    std::cout << "未知事件类型: " << static_cast<int>(event.event_type()) << std::endl;
            }
        };

    watcher.reset(new etcd::Watcher(*client, key, callback, true));
    watcher->Wait([client, key, url, &watcher](bool cancelled) {
        if (cancelled)
        {
            std::cout << "watcher's reconnect loop stopped as been cancelled" << std::endl;
            return;
        }
        get(client, watcher, key, url); // 重新浏览目录变化
    });
}

int main()
{
    const std::string url = "http://192.168.237.129:2379";
    const std::string key = "/password/";
    std::shared_ptr<etcd::Client> client = std::make_shared<etcd::Client>(url);
    std::shared_ptr<etcd::Watcher> watcher = nullptr;
    get(client, watcher, key, url);

    std::cout << "回车结束，租约失效" << std::endl;
    getchar();
    return 0;
}
