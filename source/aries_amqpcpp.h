#pragma once
#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <ev.h>
#include <mutex>
#include <thread>
#include <condition_variable>

namespace AriesAMQP {

    const std::string DIRECT_EXCHANGE_TYPE = "direct";
    const std::string DELAYED_EXCHANGE_TYPE = "delayed";
    const std::string FANOUT_EXCHANGE_TYPE = "fanout";
    const std::string TOPIC_EXCHANGE_TYPE = "topic";
    const std::string HEADERS_EXCHANGE_TYPE = "headers";

    struct declare_setting {
        std::string _exchange;
        std::string _queue;
        std::string _binding_key;
        std::string _exchange_type = DIRECT_EXCHANGE_TYPE;
        size_t _delayed_ttl = 0;

        std::string dlx_exchange() const;
        std::string dlx_queue() const;
        std::string dlx_binding_key() const;
    };

    using MessageCallBack = std::function<void(const char *, size_t)>;
    extern AMQP::ExchangeType exchange_type(const std::string& type);
    class MQClient {
        public:
            using ptr = std::shared_ptr<MQClient>;
            MQClient(const std::string &url);
            ~MQClient();
            // 声明exchange、queue
            void _declare(const declare_setting &setting, const AMQP::Table& argx, bool is_dlx = false);
            void declare(const declare_setting &setting);
            bool produce(const std::string &exchange, const std::string &binding_key, const std::string &msg);
            void consume(const std::string &queue, const MessageCallBack& callback);
            void wait();
        private:
            static void callback(struct ev_loop *loop, ev_async *watcher, int32_t revents);
        private:
            std::mutex _declare_mutex;
            std::mutex _mutex;
            std::condition_variable _cond;
            struct ev_loop *_loop;
            struct ev_async _ev_async;
            AMQP::LibEvHandler _handler;
            AMQP::TcpConnection _connection;
            AMQP::TcpChannel _channel;
            std::thread _async_thread;
    };

    class MQProducer {
        public:
            using ptr = std::shared_ptr<MQProducer>;
            MQProducer(const MQClient::ptr &client, const declare_setting &setting);
            void produce(const std::string &msg);
        private:
            MQClient::ptr _client;
            declare_setting _setting;
    };

    class MQConsumer {
        public:
            using ptr = std::shared_ptr<MQConsumer>;
            MQConsumer(const MQClient::ptr &client, const declare_setting &setting);
            void consume(const MessageCallBack& callback);
        private:
            MQClient::ptr _client;
            declare_setting _setting;
    };

    class MQFactory {
        public:
            // 提供一个通用的工厂类实现各个不同类的创建
            template<typename R, typename... Args>
            static std::shared_ptr<R> create(Args... args) {
                return std::make_shared<R>(std::forward<Args>(args)...);
            }
    };
}