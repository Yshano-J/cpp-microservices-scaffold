#include <brpc/channel.h>
#include "cal.pb.h"

int main()
{
    brpc::ChannelOptions option;
    option.connect_timeout_ms = -1;
    option.protocol = "baidu_std";

    // 1.实例化通道对象
    brpc::Channel channel;
    channel.Init("localhost:9000", &option);
    // 2.实例化计算服务对象--用于发起rpc请求
    cal::CalculatorService::Stub stub(&channel);
    brpc::Controller controller;

    // 3.实例化请求对象
    cal::AddRequest request;
    cal::AddReply response;
    request.set_a(1);
    request.set_b(2);

    stub.Add(&controller, &request, &response, nullptr);
    if(controller.Failed() == true) {
        std::cout << "Add failed: " << controller.ErrorText() << std::endl;
        return 1;
    }
    std::cout << "Add result: " << response.result() << std::endl;

    return 0;
}