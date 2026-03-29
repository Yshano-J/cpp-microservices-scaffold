#include "../../source/aries_etcd_service_register.h"

int main()
{
    aries_log::global_logger_init();

    AriesService::SvcProvider provider("http://192.168.237.129:2379", "user", "192.168.237.129:9000");
    provider.svcRegister();
    getchar();
    return 0;
}