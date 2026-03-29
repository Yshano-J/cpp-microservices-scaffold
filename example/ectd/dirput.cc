#include "etcd/Client.hpp"
#include "etcd/Response.hpp"
#include "etcd/KeepAlive.hpp"
#include "etcd/Value.hpp"
#include "etcd/Watcher.hpp"
#include <iostream>

void put(std::shared_ptr<etcd::KeepAlive>alive,  std::string key, const std::string &value, const std::string &url)
{
    // 实例化etcd客户端对象
    etcd::Client client(url);
    // 创建租约
    etcd::Response lease = client.leasegrant(3).get();
    if (!lease.is_ok())
    {
        std::cout << "leasegrant failed" << std::endl;
        return;
    }
    // 获取租约ID
    auto lease_id = lease.value().lease();
    auto response = client.put(key, value, lease_id).get();
    // auto response = client.put("/password/", "00000000000000", lease_id).get();

    if (!response.is_ok())
    {
        std::cout << "put failed" << std::endl;
        return;
    }
    // 保活处理函数
    auto handler = [&](std::exception_ptr ep)
    {
        put(alive, key, value, url);
    };
    // 保活
    alive.reset(new etcd::KeepAlive(url, handler, 3, lease_id));
}

int main()
{
    const std::string url = "http://192.168.237.129:2379";
    const std::string key = "/password/";
    const std::string value = "CXFTmoy732892ryuvmoanBIBVovnauie";
    // 保活对象
    std::shared_ptr<etcd::KeepAlive> alive = nullptr;

    put(alive, key, value, url);
    std::cout << "回车结束，租约失效" << std::endl;
    getchar();
    return 0;
}
