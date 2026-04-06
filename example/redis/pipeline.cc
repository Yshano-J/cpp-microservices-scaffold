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

        {
            // transaction与pipeline操作一致
            auto pipe = redis.pipeline(false);
            pipe.set("key1","value1");
            pipe.set("key2","value2");
            pipe.set("key3","value3");
            pipe.set("key4","value4");
            // pipe.get("key1"); // 获取不到结果
            auto reply = pipe.exec(); // 必须执行exec才能得到结果

            // reply.get<bool>(0);  // pipe.set("key1","value1");
            // reply.get<sw::redis::OptionalString>(4); // pipe.get("key1");
        }

    }
    catch (sw::redis::Error &e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}