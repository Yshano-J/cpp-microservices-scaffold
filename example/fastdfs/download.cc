#include <fastcommon/logger.h>
#include <fastdfs/fdfs_client.h>
#include <iostream>

int main(int argc, char *argv[])
{
    // 1. 初始化⽇志输出模块
    g_log_context.log_level = LOG_ERR;
    log_init();
    // 2. 初始化客⼾端全局配置（1. 从配置⽂件进⾏初始化/ 2. 从缓冲区进⾏初始化）
    int ret = fdfs_client_init("./client.conf");
    if (ret != 0)
    {
        std::cout << "初始化全局配置失败: " << STRERROR(ret) << std::endl;
        return -1;
    }
    // 3. 获取tracker服务器连接句柄
    auto tracker_server = tracker_get_connection();
    if (tracker_server == nullptr)
    {
        std::cout << "获取tracker服务器连接句柄失败" << std::endl;
        return -1;
    }
    // 4. 进⾏⽂件的上传/下载（可以⼿动获取⼀个storage节点连接然后进⾏操作）
    int64_t file_size;
    ret = storage_download_file_to_file1(tracker_server, nullptr, argv[1], "./makefile.bak", &file_size);
    if (ret != 0)
    {
        std::cout << "下载⽂件失败: " << STRERROR(ret) << std::endl;
        return -1;
    }
    std::cout << "⽂件下载成功: " << file_size << std::endl;

    // 5. 释放资源（关闭tracker服务器连接句柄， 销毁客⼾端全局配置）
    tracker_close_connection(tracker_server);
    fdfs_client_destroy();
    return 0;
}