#include "../../source/util.h"
#include <iostream>
void serialize_test()
{
    Json::Value val;
    val["name"] = "zhangsan";
    val["age"] = 19;
    val["score"].append(78.0);
    val["score"].append(90.5);
    val["score"].append(95.0);
    auto ret = aries_util::JSON::serialize(val);
    if(ret == std::nullopt)
        return;
    std::cout << *ret << std::endl;
}

void deserialize_test()
{
    std::string info = R"({"age":19,"name":"zhangsan","score":[78.0,90.5,95.0]})";
    auto ret = aries_util::JSON::deserialize(info);
    if (ret == std::nullopt)
        return;
    std::cout << (*ret)["name"].asString() << std::endl;
    std::cout << (*ret)["age"].asInt() << std::endl;
    if (!(*ret)["score"].isNull() && (*ret)["score"].isArray())
    {
        int size = (*ret)["score"].size();
        for (int i = 0; i < size; i++)
        {
            std::cout << (*ret)["score"][i].asDouble() << " ";
        }
        std::cout << std::endl;
    }
}

int main()
{
    serialize_test();
    deserialize_test();
    return 0;
}