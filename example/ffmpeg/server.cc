#include <httplib.h>

int main()
{
    httplib::Server server;
    server.set_mount_point("/", "./wwwroot/video");
    server.listen("0.0.0.0", 9000);
    return 0;
}
