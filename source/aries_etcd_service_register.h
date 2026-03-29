#include "etcd/Client.hpp"
#include "etcd/Response.hpp"
#include "etcd/KeepAlive.hpp"
#include "etcd/Value.hpp"
#include "etcd/Watcher.hpp"
#include "util.h"
#include "aries_log.h"
#include <iostream>

namespace AriesService {
    extern void wait_for_connection(etcd::Client &client);

    class SvcProvider {
        public:
            SvcProvider(const std::string& reg_center_addr, const std::string& service_name, const std::string& service_addr);
            void svcRegister();
            std::string make_key();
        private:
            std::string _reg_center_addr; // 注册中心地址
            std::string _instance_id; // 实例ID
            std::string _service_name; // 服务名称
            std::string _service_addr; // 服务地址
            std::shared_ptr<etcd::KeepAlive> _keep_alive; // 保活对象
    };

    class SvcWatcher {
        public:
            using ModifyCallback = std::function<void(std::string, std::string)>;
            SvcWatcher(const std::string &reg_center_addr, ModifyCallback &&online_callback, ModifyCallback &&offline_callback);
            bool watch();

        private:
            std::string _reg_center_addr; // 注册中心地址
            ModifyCallback _online_callback;// 服务上线回调函数
            ModifyCallback _offline_callback; // 服务下线回调函数
            std::shared_ptr<etcd::Watcher> _watcher; // 监控对象
    };
}
