#include "aries_amqpcpp.h"
#include "aries_log.h"

namespace AriesAMQP
{
    std::string declare_setting::dlx_exchange() const {
        return "dlx_" + _exchange;
    }
    std::string declare_setting::dlx_queue() const {
        return "dlx_" + _queue;
    }
    std::string declare_setting::dlx_binding_key() const {
        return "dlx_" + _binding_key;
    }
    extern AMQP::ExchangeType exchange_type(const std::string &type)
    {
        if(type == DIRECT_EXCHANGE_TYPE) {
            return AMQP::ExchangeType::direct;
        } else if(type == FANOUT_EXCHANGE_TYPE) {
            return AMQP::ExchangeType::fanout;
        } else if(type == HEADERS_EXCHANGE_TYPE) {
            return AMQP::ExchangeType::headers;
        } else if(type == TOPIC_EXCHANGE_TYPE) {
            return AMQP::ExchangeType::topic;
        } else if(type == DELAYED_EXCHANGE_TYPE) {
            return AMQP::ExchangeType::direct;
        } else {
            return AMQP::ExchangeType::direct;
        }
    }

    MQClient::MQClient(const std::string &url)
        : _loop(EV_DEFAULT)
        , _handler(_loop)
        , _connection(&_handler, AMQP::Address(url))
        , _channel(&_connection)
        , _async_thread(std::thread([this](){ ev_run(_loop); }))
    { }

    void MQClient::callback(struct ev_loop *loop, ev_async *watcher, int32_t revents)
    {
        ev_break(loop); // 这个接口不能跨线程调用
    }

    MQClient::~MQClient()
    {
        ev_async_init(&_ev_async, MQClient::callback);
        ev_async_start(_loop, &_ev_async);
        ev_async_send(_loop, &_ev_async);
        _async_thread.join();
    }

    void MQClient::_declare(const declare_setting &setting, const AMQP::Table &args, bool is_dlx){
        std::unique_lock<std::mutex> declare_lock(_declare_mutex); // 用于保护声明操作
        std::unique_lock<std::mutex> lock(_mutex); // 用于保护等待条件变量

        std::string exchange, queue, binding_key;
        if(is_dlx) {
            exchange = setting.dlx_exchange();
            queue = setting.dlx_queue();
            binding_key = setting.dlx_binding_key();
        } else {
            exchange = setting._exchange;
            queue = setting._queue;
            binding_key = setting._binding_key;
        }
        AMQP::ExchangeType type = exchange_type(setting._exchange_type);
        _channel.declareExchange(exchange, type)
            .onSuccess([=](){
                INFO("declare exchange {} success", exchange);
                _channel.declareQueue(queue, args)
                    .onSuccess([=](){
                        INFO("declare queue {} success", queue);
                        _channel.bindQueue(exchange, queue, binding_key)
                            .onSuccess([=](){
                                INFO("bind queue {} to exchange {} success", queue, exchange);
                                _cond.notify_one();  // 声明成功，通知等待线程
                            })
                            .onError([=](const char* msg){
                                ERROR("bind queue to exchange failed: {}", msg);
                                _cond.notify_one();
                                abort();
                            });
                        })
                    .onError([=](const char *msg){
                        ERROR("declare queue failed: {}", msg);
                        _cond.notify_one();
                        abort(); 
                    });
            })
            .onError([=](const char *msg){
                ERROR("declare exchange failed: {}", msg);
                _cond.notify_one();
                abort(); 
            });
        _cond.wait(lock);
    }

    void MQClient::declare(const declare_setting &setting) {
        AMQP::Table args;
        if (setting._exchange_type == DELAYED_EXCHANGE_TYPE)
        {
            _declare(setting, args, true);  // 声明死信exchange、queue、binding_key
            args["x-dead-letter-exchange"] = setting.dlx_exchange();
            args["x-dead-letter-routing-key"] = setting.dlx_binding_key();
            args["x-message-ttl"] = setting._delayed_ttl;
            // DBG("declare delayed exchange {} success", setting._exchange);
        }
        _declare(setting, args, false);  // 声明主exchange、queue、binding_key
    }
    bool MQClient::produce(const std::string &exchange, const std::string &binding_key, const std::string &msg) {
        return _channel.publish(exchange, binding_key, msg);
    }
    void MQClient::consume(const std::string &queue, const MessageCallBack &callback) {
        std::unique_lock<std::mutex> declare_lock(_declare_mutex);
        std::unique_lock<std::mutex> lock(_mutex);
        _channel.consume(queue)
            .onSuccess([=](){ 
                INFO("consume success");
                _cond.notify_one(); 
            })
            .onMessage([=](const AMQP::Message &message,
                           uint64_t delivery_tag,
                           bool redelivered){
                    callback(message.body(), message.size());
            })
            .onError([=](const char *message){
                WARNING("consume message failed: {}", message);
                _cond.notify_one();
                abort(); 
            });
        _cond.wait(lock);
    }

    void MQClient::wait() {
        _async_thread.join();
    }

    MQProducer::MQProducer(const MQClient::ptr & client, const declare_setting &setting)
        : _setting(setting), _client(client)
    {
        _client->declare(setting);
    }
    void MQProducer::produce(const std::string &msg) {
        _client->produce(_setting._exchange, _setting._binding_key, msg);
    }

    MQConsumer::MQConsumer(const MQClient::ptr & client, const declare_setting &setting)
        : _client(client), _setting(setting)
    {
        _client->declare(setting);
    }

    void MQConsumer::consume(const MessageCallBack &callback) 
    {
        if (_setting._exchange_type == DELAYED_EXCHANGE_TYPE) {
            _client->consume(_setting.dlx_queue(), callback);
        } else {
            _client->consume(_setting._queue, callback);
        }
    }
}
