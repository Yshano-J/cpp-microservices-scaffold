#pragma once
#include <iostream>
#include <vector>
#include <optional>
#include <sstream>
extern "C"
{
#include <fastdfs/fdfs_client.h>
#include <fastcommon/logger.h>
}

namespace AriesFastdfs
{
    struct fdfs_settings
    {
        std::vector<std::string> trackers;
        int connect_timeout = 30;
        int network_timeout = 30;
        bool use_connection_pool = true;
        int connection_pool_max_idle_time = 3600;
    };
    class FDFSClient
    {
        public:
            static void init(const fdfs_settings &settings);// 初始化客⼾端全局配置
            static void destroy(); // 销毁全局配置
            static std::optional<std::string> upload_from_buff(const std::string &buff); // 从缓冲区上传文件
            static std::optional<std::string> upload_from_file(const std::string &path); // 从文件上传文件
            static bool download_to_buff(const std::string &file_id, std::string &buff); // 下载文件到缓冲区
            static bool download_to_file(const std::string &file_id, const std::string &path); // 下载文件到文件
            static bool remove(const std::string &file_id); // 删除文件
    };
}