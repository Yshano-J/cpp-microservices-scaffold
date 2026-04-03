#pragma once
#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <ev.h>
#include <mutex>
#include <thread>
#include <condition_variable>

namespace AriesAMQP {
    struct declare_setting {
        const std::string _exchange;
        const std::string _queue;
        const std::string _binding_key;
        struct ev_loop *_loop;
        struct ev_async _ev_async;
    };

    class MQClient {
        public:
            MQClient(const declare_setting &setting);
            void declare();
            void producer();
            void consumer();
            void start();
            void wait();

        private:
            std::mutex _mutex;
            std::condition_variable _cond;
            declare_setting _setting;
            AMQP::LibEvHandler _handler;
            AMQP::TcpConnection _connection;
            AMQP::TcpChannel _channel;
            std::thread _async_thread;
    };

    class MQProducer {
        public:

        private:
    };

    class MQConsumer {
        public:

        private:
    };
    
}