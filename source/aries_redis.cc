#include "aries_redis.h"

namespace AriesRedis {
    std::shared_ptr<sw::redis::Redis> RedisFactory::create(const redis_settings& settings)
    {
        sw::redis::ConnectionOptions connOpts = {
            .host = settings._host,
            .port = settings._port,
            .password = settings._password,
            .db = settings._db,
        };
        sw::redis::ConnectionPoolOptions pool_opts = {
            .size = settings._connection_pool_size,
            .connection_idle_time = std::chrono::milliseconds(3600000) // 3600秒
        };
        return std::make_shared<sw::redis::Redis>(connOpts, pool_opts);
    }
}