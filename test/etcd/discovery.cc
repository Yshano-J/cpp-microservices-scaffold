#include "../../source/aries_etcd_service_register.h"

void online(const std::string &service_name, const std::string &service_addr)
{
   INFO("service: {} is online, addr: {}", service_name, service_addr);
}

void offline(const std::string &service_name, const std::string &service_addr)
{
    INFO("service: {} is offline, addr: {}", service_name, service_addr);
}

int main()
{    
    aries_log::global_logger_init();

    AriesService::SvcWatcher watcher("http://192.168.237.129:2379", online, offline);
    watcher.watch();
    getchar();
    return 0;
}