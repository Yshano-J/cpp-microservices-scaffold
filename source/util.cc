
#include "util.h"
#include "aries_log.h"
namespace aries_util
{
    std::optional<std::string> JSON::serialize(const Json::Value &val)
    {
        Json::StreamWriterBuilder builder;
        builder["indentation"] = "";
        std::unique_ptr<Json::StreamWriter> sw(builder.newStreamWriter());
        std::stringstream ss;
        int ret = sw->write(val, &ss);
        if (ret != 0)
        {
            ERROR("serialize failed!");
            return std::nullopt;
        }
        return ss.str();
    }
    std::optional<Json::Value> JSON::deserialize(const std::string &input)
    {
        Json::CharReaderBuilder charBuilder;
        std::unique_ptr<Json::CharReader> cr(charBuilder.newCharReader());
        Json::Value val;
        std::string err_msg;
        bool res = cr->parse(input.c_str(), input.c_str() + input.size(), &val, &err_msg);
        if (res == false)
        {
            ERROR("deserialize failed! {}", err_msg);
            return std::nullopt;
        }
        return val;
    }
}
