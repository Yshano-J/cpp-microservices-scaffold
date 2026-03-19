#include <iostream>
#include <string>
#include <gflags/gflags.h>

DEFINE_bool(reuse_addr, true, "是否设置地址重用");
DEFINE_int32(listen_port, 8080, "服务器端口");
DEFINE_string(listen_ip, "0.0.0.0", "服务器ip");

int main(int argc, char *argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    std::cout << FLAGS_listen_ip << std::endl;
    std::cout << FLAGS_listen_port << std::endl;
    std::cout << FLAGS_reuse_addr << std::endl;

    return 0;
}