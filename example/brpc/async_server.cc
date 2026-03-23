#include <brpc/server.h>
#include <thread>
#include "cal.pb.h"

class CalculatorServiceImpl : public cal::CalculatorService
{
public:
    CalculatorServiceImpl() {}
    ~CalculatorServiceImpl() {}
    void Add(::google::protobuf::RpcController *controller,
                     const ::cal::AddRequest *request,
                     ::cal::AddReply *response,
                     ::google::protobuf::Closure *done) override
    {
        std::thread thread([=](){
            brpc::ClosureGuard guard(done);
            response->set_result(request->a() + request->b());
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });
        thread.detach();
        std::cout << "============" << std::endl;
    }
};

int main()
{
    // 1.实例化计算服务对象
    CalculatorServiceImpl calculator_service_impl;
    brpc::ServerOptions options;
    options.idle_timeout_sec = -1;
    // 2.实例化服务器对象
    brpc::Server server;
    // 3.添加服务
    server.AddService(&calculator_service_impl, brpc::SERVER_DOESNT_OWN_SERVICE);
    // 4.启动服务器
    server.Start(9000, &options);
    // 5.等待服务器退出
    server.Join();
    // 6.等待服务器退出
    server.RunUntilAskedToQuit();
    return 0;
}
