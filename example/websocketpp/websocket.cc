#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <iostream>

using websocket_t = websocketpp::server<websocketpp::config::asio>;

// 创建回调函数
void onOpen(websocket_t* server, const websocketpp::connection_hdl& hdl)
{
    std::cout << "server running" << std::endl;
}
void OnClose(websocket_t* server, const websocketpp::connection_hdl& hdl)
{
    std::cout << "server close" << std::endl;
}
void onHttp(websocket_t *server, const websocketpp::connection_hdl &hdl)
{
    std::cout << "Get Http Request" << std::endl;
    // 获取连接
    auto conn = server->get_con_from_hdl(hdl);
    // 获取请求
    const auto& req = conn->get_request();
    std::cout << req.get_method() << std::endl;
    std::cout << req.get_uri() << std::endl;
    std::cout << req.get_body() << std::endl;
    for (auto &it : req.get_headers()) {
        std::cout << it.first << " = " << it.second << std::endl;
    }
    // 响应体
    conn->set_status(websocketpp::http::status_code::ok, "hello world");
    conn->set_body("<html><body><h1>hello world</h1></body></html>");
    conn->append_header("Content-Type", "text/html");
}
void onMessage(websocket_t* server, const websocketpp::connection_hdl &hdl, websocket_t::message_ptr msg)
{
    auto conn = server->get_con_from_hdl(hdl);
    std::cout << "Get Message Request" << std::endl;
    std::cout << msg->get_payload() << std::endl;
    // 响应体
    conn->send(msg->get_payload(), websocketpp::frame::opcode::text);
}   

int main() 
{
    websocket_t server;
    server.clear_access_channels(websocketpp::log::alevel::all);
    // 设置回调
    server.set_open_handler(std::bind(&onOpen, &server, std::placeholders::_1));
    server.set_close_handler(std::bind(&OnClose, &server, std::placeholders::_1));
    server.set_http_handler(std::bind(&onHttp, &server, std::placeholders::_1));
    server.set_message_handler(std::bind(&onMessage, &server, std::placeholders::_1, std::placeholders::_2));
    // 初始化
    server.init_asio();
    // 设置地址复用
    server.set_reuse_addr(true);
    // 监听端口
    server.listen(9000);
    // 启动接受
    server.start_accept();
    // 运行
    server.run();
    return 0;
}