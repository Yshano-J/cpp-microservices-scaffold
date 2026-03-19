#include <aries_log.h>
#include <gflags/gflags.h>
// 1.gflags
DEFINE_bool(async, false, "是否启用异步日志器");
DEFINE_int32(level, 1, "日志等级");
DEFINE_string(format, "[%Y-%m-%d %H:%M:%S][%-8l]%v", "日志输出格式");
DEFINE_string(path, "stdout", "日志输出路径");

int main(int argc, char *argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    aries_log::log_settings settings = {
        .async = FLAGS_async,
        .level = FLAGS_level,
        .format = FLAGS_format,
        .path = FLAGS_path};
        
    aries_log::global_logger_init(settings);
    DBG("{}打了{}把游戏", "臧灏舒", 18);
    INFO("{}打了{}把游戏", "臧灏舒", 18);
    ERROR("{}打了{}把游戏", "臧灏舒", 18);
    WARNING("{}打了{}把游戏", "臧灏舒", 18);
    return 0;
}
