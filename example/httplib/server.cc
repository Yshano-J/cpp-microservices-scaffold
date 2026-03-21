#include <httplib.h>
#include <iostream>

int main()
{
    httplib::Server svr;
    svr.Get("/hello", [](const httplib::Request& req, httplib::Response& rsp) {
        std::cout << req.method << std::endl;
        std::cout << req.path << std::endl;
        for (auto& it : req.headers) {
            std::cout << it.first << " = " << it.second << std::endl;
        }
        for (auto &it : req.params) {
            std::cout << it.first << " = " << it.second << std::endl;
        }
        std::string html_body = "<html><body><h1>hello world</h1></body></html>";
        rsp.set_content(html_body, "text/html");
        rsp.set_header("Content-Length", std::to_string(html_body.size()));
        rsp.status = 200;
        rsp.body = html_body;
    });
    svr.Get(R"(/numbers/(\d+))", [&](const httplib::Request &req, httplib::Response &rsp)
            {
        std::cout << req.method << std::endl;
        std::cout << req.path << std::endl;
        for (auto& it : req.headers) {
            std::cout << it.first << " = " << it.second << std::endl;
        }
        for (auto &it : req.matches)
        {
            std::cout << it << std::endl;
        }
        auto numbers = req.matches[1];
        rsp.set_content(numbers, "text/plain"); 
        std::string html_body = "<html><body><h1>numbers: " + std::to_string(std::stoi(numbers)) + "</h1></body></html>";
        rsp.set_content(html_body, "text/html");
        rsp.set_header("Content-Length", std::to_string(html_body.size()));
        rsp.status = 200;
        rsp.body = html_body; });

    svr.listen("0.0.0.0", 9000);

    return 0;
}
