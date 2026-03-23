#include <brpc/channel.h>
#include "cal.pb.h"

using call_back_t = std::function<void()>;
struct CallBack{
    call_back_t _cb;
};

void AddCallback(const CallBack cb)
{
    cb._cb();
}

int main()
{
    brpc::ChannelOptions option;
    option.connect_timeout_ms = -1;
    option.protocol = "baidu_std";

    // 1.实例化通道对象--用于发起rpc请求
    brpc::Channel channel;
    channel.Init("localhost:9000", &option);
    // 2.实例化计算服务对象--用于发起rpc请求
    cal::CalculatorService::Stub stub(&channel);
    brpc::Controller* controller = new brpc::Controller();
    controller->set_timeout_ms(2000);
    // 3.实例化请求对象
    cal::AddRequest *request = new cal::AddRequest();
    cal::AddReply* response = new cal::AddReply();
    request->set_a(1);
    request->set_b(2);
    // 4.发起异步rpc请求
    // google::protobuf::Closure* done = brpc::NewCallback(AddCallback, controller, request, response);
    CallBack cb;
    cb._cb = [=](){
        std::unique_ptr<brpc::Controller> cntl_guard(controller);
        std::unique_ptr<cal::AddRequest> request_guard(request);
        std::unique_ptr<cal::AddReply> response_guard(response);

        if(cntl_guard->Failed() == true) {
            std::cout << "Add failed: " << cntl_guard->ErrorText() << std::endl;
            return;
        }
        std::cout << "Add result: " << response_guard->result() << std::endl;
    };
    
    google::protobuf::Closure* done = brpc::NewCallback(AddCallback, cb);

    stub.Add(controller, request, response, done);
    std::cout << "===========================" << std::endl; // 先执行
    getchar();
    return 0;
}