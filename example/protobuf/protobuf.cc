#include <iostream>
#include <google/protobuf/util/json_util.h>
#include "test.pb.h"

std::string serialize_test()
{
    example::Person person;
    person.set_name("Jack");
    person.set_age(28);
    person.set_email("qq.com");
    auto score_map = person.mutable_scores();
    score_map->insert({"Ch", 88});
    score_map->insert({"Mt", 77});
    score_map->insert(google::protobuf::MapPair<std::string, float>("En", 99));
    std::string pstr = person.SerializeAsString();
    std::cout << "pstr: " << pstr << std::endl;
    return pstr;
}

void deserialize_test(const std::string &pstr)
{
    example::Person tmp;
    // 针对上边序列化的⼆进制数据传进⾏反序列化
    bool ret = tmp.ParseFromString(pstr);
    if (ret == false)
    {
        std::cout << "ParseFromString Failed!\n";
        return;
    }
    std::cout << "name: " << tmp.name() << std::endl;
    std::cout << "age: " << tmp.age() << std::endl;
    std::cout << "email: " << tmp.email() << std::endl;

    // score字段是⼀个google::protobuf::Map字段，可以通过迭代器进⾏访问
    auto beg = tmp.scores().begin();
    auto end = tmp.scores().end();
    for (; beg != end; ++beg)
    {
        std::cout << beg->first << " = " << beg->second << std::endl;
    }
}

void json2protobuf()
{
    const std::string &json_str = R"({
        "name": "Jack",
        "age": 28,
        "email": "qq.com",
        "skills": ["C++", "Python"],
        "scores": {
            "Ch": 88.0,
            "Mt": 77.0,
            "En": 99.0
        }
    })";
    example::Person tmp;
    // bool ret = tmp.ParseFromString(json_str);
    auto ret = google::protobuf::util::JsonStringToMessage(json_str, &tmp);
    if (ret.ok() == false)
    {
        std::cout << "JsonStringToMessage Failed!\n";
        return;
    }

    std::cout << "name: " << tmp.name() << std::endl;
    std::cout << "age: " << tmp.age() << std::endl;
    std::cout << "email: " << tmp.email() << std::endl;
    std::cout << "skills: " << tmp.skills(0) << std::endl;
    std::cout << "skills: " << tmp.skills(1) << std::endl;
    auto beg = tmp.scores().begin();
    auto end = tmp.scores().end();
    for (; beg != end; ++beg)
    {
        std::cout << beg->first << " = " << beg->second << std::endl;
    }
}

void protobuf2json()
{
    example::Person tmp;
    tmp.set_name("Jack");
    tmp.set_age(28);
    tmp.set_email("qq.com");
    auto score_map = tmp.mutable_scores();
    score_map->insert({"Ch", 88});
    score_map->insert({"Mt", 77});
    score_map->insert(google::protobuf::MapPair<std::string, float>("En", 99));
    std::string json_str;
    google::protobuf::util::JsonPrintOptions options;
    options.add_whitespace = true;
    auto ret = google::protobuf::util::MessageToJsonString(tmp, &json_str, options);
    if (ret.ok() == false)
    {
        std::cout << "MessageToJsonString Failed!\n";
        return;
    }
    std::cout << "json_str: " << json_str << std::endl;
}

int main()
{
    std::string pstr = serialize_test();
    std::cout << "============================\n";
    deserialize_test(pstr);
    std::cout << "============================\n";
    json2protobuf();
    std::cout << "============================\n";
    protobuf2json();
    return 0;
}