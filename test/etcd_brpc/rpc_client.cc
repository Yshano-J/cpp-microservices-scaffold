#include "../../source/aries_brpc.h"
#include "cal.pb.h"
#include "../../source/aries_etcd_service_register.h"


int main()
{
    aries_log::global_logger_init();
    std::string register_addr = "http://192.168.237.129:2379";
    std::string service_name = "cal";

    brpc::ChannelOptions option;
    option.connect_timeout_ms = -1;
    option.protocol = "baidu_std";

    // 1.实例化通道对象--用于发起rpc请求
    AriesBrpc::ServiceChannels service;
    service.set_watch("cal");

    auto online = std::bind(&AriesBrpc::ServiceChannels::insert, &service, std::placeholders::_1, std::placeholders::_2); // 服务上线时调用
    auto offline = std::bind(&AriesBrpc::ServiceChannels::remove, &service, std::placeholders::_1, std::placeholders::_2); // 服务下线时调用
    AriesService::SvcWatcher watcher(register_addr, online, offline);
    watcher.watch();

    AriesBrpc::ChannelPtr channel_ptr;
    while (!channel_ptr)
    {
        WARNING("get channel failed");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        channel_ptr = service.get("cal"); // 获取服务结点
    }
    INFO("get channel success, use service and return result immediately");

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
            WARNING("Add failed: {}", cntl_guard->ErrorText());
            return;
        }
        INFO("Add result: {}", response_guard->result()); });

    cal::CalculatorService::Stub stub(channel_ptr.get());
    stub.Add(controller, request, response, closure);
    INFO("=============先执行==============="); // 先执行
    getchar();
    return 0;
}