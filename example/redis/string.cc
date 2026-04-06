#include <sw/redis++/redis.h>
#include <iostream>
#include <string>
#include <cassert>

int main()
{
    try {
        sw::redis::ConnectionOptions connOpts = {
            .host = "192.168.237.129",
            .port = 6379,
            .password = "123456",
            .db = 0,
            .keep_alive = true
        };
        sw::redis::ConnectionPoolOptions pool_opts = {
            .size = 3,
            .connection_idle_time = std::chrono::milliseconds(3600000) // 3600秒
        };

        sw::redis::Redis redis(connOpts, pool_opts );
        bool ret = redis.set("name", "zhangsan");
        if(!ret) {
            std::cout << "设置键值对错误"<< std::endl;
            return -1;
        }

        auto val = redis.get("name");
        if(!val) {
            std::cout << "获取键值对错误"<< std::endl;
            return -1;
        }
        std::cout << *val <<std::endl;

        redis.del("name");
        redis.set("age", "18");
        redis.incrby("age", 1);
        std::cout << *(redis.get("age")) << std::endl;
        redis.decrby("age", 1);
        std::cout << *(redis.get("age")) << std::endl;
        redis.del("age");
    } catch(sw::redis::Error &e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}