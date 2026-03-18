#include "aries_log.h"

namespace aries_log
{
    std::shared_ptr<spdlog::logger> g_logger; // 定义全局日志器
    void global_logger_init(const log_settings &log)
    {
        // 判断日志器类型：async/sync
        if (log.async == true) // async
        {
            if (log.path == "stdout")
                g_logger = spdlog::stdout_color_mt<spdlog::async_factory>("stdout_logger"); // 输出到标准输出
            else
                g_logger = spdlog::basic_logger_mt<spdlog::async_factory>("file_logger", log.path); // 输出到单个文件
        }
        else // sync
        {
            if (log.path == "stdout")
                g_logger = spdlog::stdout_color_mt("stdout_logger"); // 输出到标准输出
            else
                g_logger = spdlog::basic_logger_mt("file_logger", log.path); // 输出到单个文件
        }
        // 设置日志等级
        g_logger->set_level(spdlog::level::level_enum(log.level));
        // 设置日志模式
        g_logger->set_pattern(log.format);
    }
}