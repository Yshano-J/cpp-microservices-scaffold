#include <sw/redis++/redis.h>
#include <sw/redis++/queued_redis.h>
#include <iostream>
#include <string>
#include <cassert>
#include <unistd.h>

int main()
{
    try
    {
        sw::redis::ConnectionOptions connOpts = {
            .host = "192.168.237.129",
            .port = 6379,
            .password = "123456",
            .db = 0,
            .keep_alive = true};
        sw::redis::ConnectionPoolOptions pool_opts = {
            .size = 3,
            .connection_idle_time = std::chrono::milliseconds(3600000) // 3600秒
        };
        sw::redis::Redis redis(connOpts, pool_opts);
        redis.set("score", "0");
        
        {
            // transaction与pipeline操作一致
            auto transaction = redis.transaction(false);
            auto r = transaction.redis(); // r与pipe共用一个连接
            // transaction中不能使用watch; transaction操作不能立即获取结果

            while (true)
            {
                try
                {
                    r.watch("score");
                    bool ret = r.exists("score");
                    if(!ret)
                        break;

                    transaction.incrby("score", 1).exec();
                    // 成功执行
                    break;
                }
                catch (const sw::redis::WatchError &e)
                {
                    // key被修改了
                    std::cout << e.what() << std::endl;
                }
                catch (const sw::redis::Error &e)
                {
                    // 其他错误
                    std::cout << e.what() << std::endl;
                    throw;
                }
            }
        }
    }
    catch (sw::redis::Error &e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}