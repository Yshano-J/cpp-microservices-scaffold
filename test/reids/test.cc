#include "../../source/aries_redis.h"

int main()
{
    try     
    {
        AriesRedis::redis_settings settings;
        settings._host = "192.168.237.129";
        settings._password = "123456";
        settings._connection_pool_size = 3;

        auto redis = AriesRedis::RedisFactory::create(settings);
        redis->set("name", "zhangsan");
        bool ret = redis->exists("name");
        if(ret){
            std::string value = *(redis->get("name"));
            std::cout << "name value :" << value << std::endl;
        }
    }catch(const sw::redis::Error& e){
        std::cout << e.what() << std::endl;
    }
    return 0;
}