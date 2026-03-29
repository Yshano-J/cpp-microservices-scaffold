#include "etcd/Client.hpp"
#include "etcd/Response.hpp"
#include "etcd/KeepAlive.hpp"
#include "etcd/Value.hpp"
#include "etcd/Watcher.hpp"
#include <iostream>

int main()
{
    const std::string url = "http://192.168.237.129:2379";
    // 实例化etcd客户端对象
    etcd::Client client(url);
    // 创建租约
    auto lease = client.leasegrant(3).get();
    if (!lease.is_ok())
    {
        std::cout << "leasegrant failed" << std::endl;
        return 1;
    }
    // 获取租约ID
    auto lease_id = lease.value().lease();
    auto response = client.put("password", "12341234", lease_id).get();
    if(!response.is_ok())
    {
        std::cout << "put failed" << std::endl;
        return 1;
    }
    // 保活处理函数
    auto handler = [](std::exception_ptr ep) {
        try
        {
            if (ep)
                std::rethrow_exception(ep);
        } catch (const std::runtime_error &e) {
            std::cerr << "Connection failure \"" << e.what() << "\"\n";
        } catch (const std::out_of_range &e) {
            std::cerr << "Lease expiry \"" << e.what() << "\"\n";
        }
    };
    // 保活
    etcd::KeepAlive alive(url, handler, 3, lease_id);
    getchar();
    return 0;
}
