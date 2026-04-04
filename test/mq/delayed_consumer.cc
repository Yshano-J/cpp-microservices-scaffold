#include "../../source/aries_amqpcpp.h"
#include "../../source/aries_log.h"

int main()
{
    aries_log::global_logger_init();
    std::string url = "amqp://admin:123456@192.168.237.129:5672/";
    AriesAMQP::declare_setting setting = {
        ._exchange = "delayed_exchange",
        ._queue = "delayed_queue",
        ._binding_key = "delayed_binding_key",
        ._exchange_type = AriesAMQP::DELAYED_EXCHANGE_TYPE,
        ._delayed_ttl = 10000
    };

    auto client = AriesAMQP::MQFactory::create<AriesAMQP::MQClient>(url);
    auto consumer = AriesAMQP::MQFactory::create<AriesAMQP::MQConsumer>(client, setting);

    consumer->consume([](const char *msg, size_t size) {
        INFO("consume: {}", msg);
    });
    client->wait();
    return 0;
}