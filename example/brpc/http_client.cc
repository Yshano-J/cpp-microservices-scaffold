#include <brpc/channel.h>
#include "cal.pb.h"

int main()
{
    brpc::ChannelOptions option;
    option.connect_timeout_ms = -1;
    option.protocol = brpc::PROTOCOL_HTTP;

    // 1.实例化通道对象
    brpc::Channel channel;
    channel.Init("localhost:9000", &option);
    // 2.实例化计算服务对象
    cal::CalculatorService::Stub stub(&channel);
    brpc::Controller controller;

    controller.http_request().set_method(brpc::HTTP_METHOD_POST);
    controller.http_request().uri().SetHttpURL("/CalculatorService/Hello");
    controller.request_attachment().append("hello world");
    channel.CallMethod(nullptr, &controller, nullptr, nullptr,  nullptr);
    if (controller.Failed()) {
        std::cout << "Add failed: " << controller.ErrorText() << std::endl;
        return 1;
    }
    std::cout << "result: " << controller.response_attachment() << std::endl;
    return 0;
}