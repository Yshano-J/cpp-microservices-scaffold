#include <sw/redis++/redis.h>
#include <iostream>
#include <string>
#include <cassert>
#include <unistd.h>

void expire_test(sw::redis::Redis& redis) 
{
    redis.set("name", "zhangsan");
    // 设置生命周期/过期时间
    redis.expire("name", std::chrono::seconds(3));
    // 移除过期时间
    redis.persist("name"); 
    int cnt = 5;
    while(cnt--) {
        std::cout << "name exists :" << (redis.exists("name")?"true":"false") << std::endl;
        sleep(1);
    }
    
    redis.del("name");
}

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

        expire_test(redis);
        
        redis.zadd("score", "a", 100);
        redis.zadd("score", "b", 100);
        redis.zadd("score", "c", 100);
        redis.zadd("score", "d", 100);
        redis.zadd("score", "e", 100);
        redis.zadd("score", "f", 100); // 权重必须相同

        sw::redis::LimitOptions limit;
        limit.offset = 0;
        limit.count = 3;

        sw::redis::BoundedInterval<std::string> interval("b", "f", sw::redis::BoundType::CLOSED);
        std::vector<std::string> members;
        redis.zrangebylex("score", interval, limit, std::back_inserter(members));
        for(auto& member:members) {
            std::cout << member << " ";
        }
        std::cout << std::endl;
        redis.flushall();
    }
    catch (sw::redis::Error &e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}