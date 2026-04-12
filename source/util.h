/*
    Util工具封装：
        1.json序列化、反序列化
*/
#pragma once

#include <jsoncpp/json/json.h>
#include <optional>
#include <fstream>
#include <sstream>
#include <random>
#include <atomic>
#include <iomanip>
#include "aries_log.h"

namespace aries_util
{
    const uint32_t DEFAULT_RANDOM_LENGTH = 16;

    class JSON {
        public:
            static std::optional<std::string> serialize(const Json::Value &val, bool styled = false);
            static std::optional<Json::Value> deserialize(const std::string& input);
    };

    class FILE_UTIL {
        public:
            static bool read(const std::string& filename, std::string& content);
            static bool write(const std::string& filename, const std::string& content);
    };
    
    class STRING {
        public:
            static size_t split(const std::string& str, const std::string& sep, std::vector<std::string>& parts);
    };

    enum StrType { CHARACTER = 0, NUMBER, MIXTYPE };

    class RANDOM {
        public:
            static std::string generateRandomString(size_t length = DEFAULT_RANDOM_LENGTH, StrType type = MIXTYPE);
            static size_t generateRandomNumberRange(size_t min, size_t max);
    };
}