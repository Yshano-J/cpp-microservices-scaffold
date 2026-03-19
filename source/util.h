/*
    Util工具封装：
        1.json序列化、反序列化
*/

#include <jsoncpp/json/json.h>
#include <optional>

namespace aries_util
{
    class JSON
    {
    public:
        static std::optional<std::string> serialize(const Json::Value &val);
        static std::optional<Json::Value> deserialize(const std::string& input);
    };
}