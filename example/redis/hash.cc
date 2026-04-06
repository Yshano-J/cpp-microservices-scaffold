#include <sw/redis++/redis.h>
#include <iostream>
#include <string>
#include <cassert>

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

        // 插入一个Hash键值对
        redis.hset("user", "name", "zhangsan");
        // 批量插入
        std::unordered_map<std::string, std::string> user = {
            {"age", "18"},
            {"gender", "male"},
            {"email", "zhangsan@example.com"}
        };
        redis.hmset("user", user.begin(), user.end());

        auto val = redis.hget("user", "name");
        if(!val) {
            return -1;
        }
        std::cout << *val << std::endl;

        std::unordered_map<std::string, std::string> results;
        redis.hgetall("user", std::inserter(results, results.begin()));
        for(auto &it : results) {
            std::cout << it.first << " " << it.second << std::endl;
        }

    }
    catch (sw::redis::Error &e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}