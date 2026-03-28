#pragma once
// 包含头文件
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async.h>
#include <memory>

namespace aries_log
{
    struct log_settings
    {
        bool async;         // 是否启用异步日志器
        int level;          // 日志等级
        std::string format; // 日志输出格式
        std::string path;   // 日志输出目标
    };
    extern std::shared_ptr<spdlog::logger> g_logger; // 声明全局日志器
    extern void global_logger_init(const log_settings &log);

#define FMT_PREFIX std::string("[{}:{}]: ")
#define DBG(fmt, ...) aries_log::g_logger->debug(FMT_PREFIX + fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define INFO(fmt, ...) aries_log::g_logger->info(FMT_PREFIX + fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define ERROR(fmt, ...) aries_log::g_logger->error(FMT_PREFIX + fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define WARNING(fmt, ...) aries_log::g_logger->warn(FMT_PREFIX + fmt, __FILE__, __LINE__, ##__VA_ARGS__)
}