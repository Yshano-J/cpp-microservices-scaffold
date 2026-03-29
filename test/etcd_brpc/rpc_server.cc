#include "../../source/aries_brpc.h"
#include <thread>
#include "cal.pb.h"
#include "../../source/aries_etcd_service_register.h"

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
        std::thread thread([=]() {
            brpc::ClosureGuard guard(done);
            response->set_result(request->a() + request->b());
            std::this_thread::sleep_for(std::chrono::seconds(1)); 
        });
        thread.detach();
    }
};

int main()
{
    // 1.实例化计算服务对象
    CalculatorServiceImpl *calculator_service = new CalculatorServiceImpl();

    auto server = AriesBrpc::RpcServerFactory::create(9000, calculator_service);

    std::string register_addr = "http://192.168.237.129:2379";
    std::string service_name = "cal";
    std::string service_addr = "192.168.237.129:9000";
    AriesService::SvcProvider provider(register_addr, service_name, service_addr);
    provider.svcRegister();

    server->RunUntilAskedToQuit();
    return 0;
}
