/*
    基于amqp-cpp库实现消息发送
*/

#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <ev.h>
#include <iostream>
#include "../../source/aries_log.h"

int main()
{
    aries_log::global_logger_init();
    // 1.实例化LibEvHandler对象--用于处理amqp事件
    auto *ev_loop = EV_DEFAULT;
    AMQP::LibEvHandler handler(ev_loop);
    // 2.实例化Tcp连接amqp服务器
    const std::string url = "amqp://admin:123456@192.168.237.129:5672/";
    AMQP::TcpConnection connection(&handler, AMQP::Address(url));
    // 3.创建通道
    AMQP::TcpChannel channel(&connection);
    const std::string_view exchange = "my-exchange";
    const std::string_view binding_key = "my-key";
    const std::string_view queue = "my-queue";
    // 4.声明队列和交换机
    channel.declareExchange(exchange, AMQP::ExchangeType::direct)
        .onSuccess([&]() {
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
                                    const std::string message = "hello world";
                                    if(channel.publish(exchange, binding_key, message))
                                    {
                                        INFO("publish message success");
                                    }
                                    else
                                    {
                                        WARNING("publish message failed");
                                        abort();
                                    } 
                                })
                                .onError([&](const char *message){
                                    WARNING("bind queue exchange failed: {}", message); 
                                    abort(); 
                                }); 
                        })
                .onError([&](const char *message){ 
                    WARNING("declare queue failed: {}", message); 
                    abort(); 
                }); 
        })
        .onError([&](const char *message){ 
            WARNING("declare exchange failed: {}", message); 
            abort(); 
        });
    // 启动事件循环，等待amqp事件发生
    ev_run(ev_loop);
    return 0;
}