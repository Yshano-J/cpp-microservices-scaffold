#include <jsoncpp/json/json.h>
#include <iostream>
#include <memory>

int main()
{
    Json::Value val;
    val["name"] = "zhangsan";
    val["age"] = 19;
    val["score"].append(78.0);
    val["score"].append(90.5);
    val["score"].append(95.0);
    // std::string str = val.toStyledString();
    // std::cout << str << std::endl;
    
    // 序列化
    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
    std::unique_ptr<Json::StreamWriter> sw(builder.newStreamWriter());
    std::stringstream ss;
    int ret = sw->write(val, &ss);
    if(ret != 0) {
        std::cerr << "write failed" << std::endl;
        return 1;
    }
    std::cout << ss.str() << std::endl;

    // 反序列化
    std::string str = ss.str();
    Json::CharReaderBuilder charBuilder;
    std::unique_ptr<Json::CharReader> cr(charBuilder.newCharReader());
    Json::Value val1;
    std::string err_msg;
    bool res = cr->parse(str.c_str(), str.c_str() + str.size(), &val1, &err_msg);
    if (res == false)
    {
        std::cout << err_msg << std::endl;
        return 1;
    }
    std::cout << val1["name"].asString() << std::endl;
    std::cout << val1["age"].asInt() << std::endl;
    if(!val1["score"].isNull() && val1["score"].isArray()) {
        int size = val1["score"].size();
        for (int i = 0; i < size;i++) {
            std::cout << val1["score"][i].asDouble() << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
