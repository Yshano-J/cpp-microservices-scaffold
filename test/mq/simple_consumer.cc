#include "../../source/aries_amqpcpp.h"
#include "../../source/aries_log.h"

int main()
{
    aries_log::global_logger_init();
    std::string url = "amqp://admin:123456@192.168.237.129:5672/";
    AriesAMQP::declare_setting setting = {
        ._exchange = "simple_exchange",
        ._queue = "simple_queue",
        ._binding_key = "simple_binding_key",
        ._exchange_type = "direct"
    };

    auto client = AriesAMQP::MQFactory::create<AriesAMQP::MQClient>(url);
    auto consumer = AriesAMQP::MQFactory::create<AriesAMQP::MQConsumer>(client, setting);

    consumer->consume([](const char *msg, size_t size) {
        INFO("consume: {}", msg);
    });
    client->wait();
    return 0;
}