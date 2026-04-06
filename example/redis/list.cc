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
        
        std::vector<std::string> skills = {"cpp", "c", "python", "java"};
        redis.lpush("skills", skills.begin(), skills.end());
        
        std::vector<std::string> tasks = {"1", "2", "3","4"};
        redis.rpush("tasks", tasks.begin(), tasks.end());

        for (int i = 0;i < skills.size();i++)
        {
            auto val =  redis.lindex("skills", i);
            std::cout << *val << " ";
        }
        std::cout << std::endl;

        std::vector<std::string> gets;
        redis.lrange("tasks", 0, -1, std::back_inserter(gets));
        for (auto val : gets)
        {
            std::cout << val << " ";
        }
        std::cout << std::endl;

        redis.lpop("tasks");
        redis.rpop("tasks");

        gets.clear();
        redis.lrange("tasks", 0, -1, std::back_inserter(gets));
        for (auto val : gets)
        {
            std::cout << val << " ";
        }
        std::cout << std::endl;
        
        redis.del("skills");
        redis.del("tasks");
    }
    catch (sw::redis::Error &e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}