/*
    基于amqp-cpp库实现消息消费
*/

#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <ev.h>
#include <iostream>
#include "../../source/aries_log.h"

void declareRegular(AMQP::TcpChannel &channel, 
    const std::string_view &exchange, 
    const std::string_view &queue, 
    const std::string_view &binding_key,
    const std::string_view &dlx_exchange,
    const std::string_view &dlx_binding_key)
{
    channel.declareExchange(exchange, AMQP::ExchangeType::direct)
        .onSuccess([&](){
            INFO("declare exchange success");

            AMQP::Table args;
            args["x-dead-letter-exchange"] = dlx_exchange;
            args["x-dead-letter-routing-key"] = dlx_binding_key;
            args["x-message-ttl"] = 10000;
            channel.declareQueue(queue, args)
                .onSuccess([&](const std::string &name,
                               uint32_t messagecount,
                               uint32_t consumercount)
                        {
                            INFO("declare queue success");
                            channel.bindQueue(exchange, queue, binding_key)
                                .onSuccess([&](){
                                    INFO("bind queue exchange success");
                                    const std::string message = "hello world";
                                    channel.publish(exchange, binding_key, message);
                                })
                                .onError([&](const char *message){
                                    WARNING("bind queue exchange failed: {}", message); 
                                    abort(); 
                                }); 
                        })
                .onError([&](const char *message){ 
                    WARNING("declare queue failed: {}", message); 
                    abort(); 
                }); })
        .onError([&](const char *message)
                 { 
            WARNING("declare exchange failed: {}", message); 
            abort(); });
}

void declareDlx(AMQP::TcpChannel &channel,
                const std::string_view &exchange,
                const std::string_view &queue,
                const std::string_view &binding_key)
{
    channel.declareExchange(exchange, AMQP::ExchangeType::direct)
        .onSuccess([&]()
                   {
            INFO("declare exchange success");
            channel.declareQueue(queue)
                .onSuccess([&](const std::string &name,
                               uint32_t messagecount,
                               uint32_t consumercount)
                        {
                            INFO("declare queue success");
                            std::cout << "声明队列成功:" << name << std::endl;
                            std::cout << "队列中有" << messagecount << "个消息" << std::endl;
                            std::cout << "队列中有" << consumercount << "个消费者" << std::endl;

                            channel.bindQueue(exchange, queue, binding_key)
                                .onSuccess([&](){
                                    INFO("bind queue exchange success");
                                })
                                .onError([&](const char *message){
                                    WARNING("bind queue exchange failed: {}", message); 
                                    abort(); 
                                }); 
                        })
                .onError([&](const char *message){ 
                    WARNING("declare queue failed: {}", message); 
                    abort(); 
                }); })
        .onError([&](const char *message)
                 { 
            WARNING("declare exchange failed: {}", message); 
            abort(); });
}

int main()
{
    aries_log::global_logger_init();
    const std::string url = "amqp://admin:123456@192.168.237.129:5672/";
    const std::string_view delayed_exchange = "delayed_exchange";
    const std::string_view delayed_binding_key = "delayed_binding_key";
    const std::string_view delayed_queue = "delayed_queue";

    const std::string_view dlx_exchange = "dlx_exchange";
    const std::string_view dlx_binding_key = "dlx_binding_key";
    const std::string_view dlx_queue = "dlx_queue";
    // 1.实例化LibEvHandler对象--用于处理amqp事件
    auto *ev_loop = EV_DEFAULT;
    AMQP::LibEvHandler handler(ev_loop);
    // 2.实例化Tcp连接amqp服务器
    AMQP::TcpConnection connection(&handler, AMQP::Address(url));
    // 3.创建通道
    AMQP::TcpChannel channel(&connection);
    // 4.声明死信队列和死信交换机
    declareDlx(channel, dlx_exchange, dlx_queue, dlx_binding_key);
    // 5.声明常规队列和常规交换机
    declareRegular(channel, delayed_exchange, delayed_queue, delayed_binding_key, dlx_exchange, dlx_binding_key);
    // 运行
    ev_run(ev_loop);
    return 0;
}