#include <sw/redis++/redis.h>
#include <iostream>
#include <string>
#include <cassert>
#include <unordered_set>

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

        // 对?序集合set的操作
        // 1. 向集合中添加?个元素
        redis.sadd("skills", "C++");
        // 2. 批量向集合中添加多个元素
        std::unordered_set<std::string> skills = {"Python", "Java", "Go"};
        redis.sadd("skills", skills.begin(), skills.end());
        // 3. 获取集合中元素的个数
        auto count = redis.scard("skills");
        std::cout << "skills size: " << count << std::endl;
        // 4. 判断集合中是否包含某个元素
        bool ret = redis.sismember("skills", "C++");
        std::cout << "skills contains C++: " << (ret ? "true" : "false") << std::endl;
        ret = redis.sismember("skills", "C");
        std::cout << "skills contains C: " << (ret ? "true" : "false") << std::endl;
        // 5. 获取集合中所有元素
        std::unordered_set<std::string> skills2;
        redis.smembers("skills", std::inserter(skills2, skills2.begin()));
        for (auto &val : skills2) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
        // 清理
        redis.del("skills");

    } catch(sw::redis::Error &e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}