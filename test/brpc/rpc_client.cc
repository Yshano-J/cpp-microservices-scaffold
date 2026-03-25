#include "../../source/aries_brpc.h"
#include "cal.pb.h"

int main()
{
    brpc::ChannelOptions option;
    option.connect_timeout_ms = -1;
    option.protocol = "baidu_std";

    // 1.实例化通道对象--用于发起rpc请求
    AriesBrpc::ServiceChannels service;
    service.set_watch("user");
    service.insert("user", "localhost:9000");
    auto channel_ptr = service.get("user");
    if (!channel_ptr)
    {
        std::cout << "get channel failed" << std::endl;
        return 1;
    }

    brpc::Controller *controller = new brpc::Controller();
    controller->set_timeout_ms(2000);
    cal::AddRequest *request = new cal::AddRequest();
    cal::AddReply *response = new cal::AddReply();
    request->set_a(51);
    request->set_b(20);
    auto closure = AriesBrpc::ClosureFactory::create([=]()
                                                     {
        std::unique_ptr<brpc::Controller> cntl_guard(controller);
        std::unique_ptr<cal::AddRequest> request_guard(request);
        std::unique_ptr<cal::AddReply> response_guard(response);

        if(cntl_guard->Failed()) {
            std::cout << "Add failed: " << cntl_guard->ErrorText() << std::endl;
            return;
        }
        std::cout << "Add result: " << response_guard->result() << std::endl; });

    cal::CalculatorService::Stub stub(channel_ptr.get());
    stub.Add(controller, request, response, closure);
    std::cout << "=============先执行==============" << std::endl; // 先执行
    getchar();
    return 0;
}