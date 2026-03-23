#include <brpc/server.h>
#include "cal.pb.h"

class CalculatorServiceImpl : public cal::CalculatorService
{
public:
    CalculatorServiceImpl() {}
    ~CalculatorServiceImpl() {}
    virtual void Add(::google::protobuf::RpcController *controller,
                    const ::cal::AddRequest *request,
                    ::cal::AddReply *response,
                    ::google::protobuf::Closure *done) override
    {
        brpc::ClosureGuard guard(done);
        response->set_result(request->a() + request->b());
    }
    virtual void Hello(::google::protobuf::RpcController *controller,
                       const ::cal::HelloRequest *request,
                       ::cal::HelloResponse *response,
                       ::google::protobuf::Closure *done) override
    {
        brpc::ClosureGuard guard(done);
        auto ctl = static_cast<brpc::Controller *>(controller);
        const auto& headers = ctl->http_request();
        std::cout << "Method: " << brpc::HttpMethod2Str(headers.method()) << std::endl;
        std::cout << "body: " << ctl->request_attachment().to_string() << std::endl;
        ctl->response_attachment().append("response " + ctl->request_attachment().to_string());
        ctl->http_response().set_status_code(200);
        ctl->http_response().set_content_type("text/plain");
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
