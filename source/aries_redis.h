#pragma once
#include <sw/redis++/redis.h>
#include <sw/redis++/queued_redis.h>
#include <iostream>

namespace AriesRedis {
    struct  redis_settings {
        int _db = 0;
        int _port = 6379;
        std::string _user = "default";
        std::string _host;
        std::string _password;
        size_t _connection_pool_size = 3;
    };

    class RedisFactory {
        public:
            static std::shared_ptr<sw::redis::Redis> create(const redis_settings& settings);
    };
}