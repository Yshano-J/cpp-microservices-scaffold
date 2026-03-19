#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async.h>
#include <iostream>

int main()
{
    // auto logger = spdlog::stdout_color_mt("stdout_logger"); // 输出到标准输出
    // auto logger = spdlog::basic_logger_mt("file_logger", "./log.dat"); // 输出到单个文件
    // auto logger = spdlog::rotating_logger_mt("rotating_logger", "rotating.dat", 1024, 3); // 循环文件（覆盖）
    // 异步输出
    auto logger = spdlog::stdout_color_mt<spdlog::async_factory>("stdout_logger"); // 输出到标准输出

    logger->set_level(spdlog::level::debug);
    logger->set_pattern("[%Y-%m-%d %H:%M:%S][%-8l] %v");
    // logger->debug("{} 的年龄是: {}", "xiaoming", 18);
    // logger->info("{} 的年龄是: {}", "wangwu", 18);
    // logger->error("{} 的年龄是: {}", "zhangsan", 18);
    // logger->critical("{} 的年龄是: {}", "kaka", 18);
    for (int i = 0; i < 10000; i++)
    {
        logger->error("have a good day! day {}", i);
    }
    return 0;
}