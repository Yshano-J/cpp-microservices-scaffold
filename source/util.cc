
#include "util.h"

namespace aries_util
{
    /*
        JSON序列化
    */
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

    /*
        文件读写
    */
    bool FILE_UTIL::read(const std::string &filename, std::string &content)
    {
        std::ifstream ifs(filename, std::ios::in | std::ios::binary);
        if(ifs.is_open() == false) {
            ERROR("read file failed! {}", filename);
            return false;
        }
        ifs.seekg(0, std::ios::end);
        content.resize(ifs.tellg());
        ifs.seekg(0, std::ios::beg);

        ifs.read(&content[0], content.size());
        ifs.close();
        return true;
    }
    bool FILE_UTIL::write(const std::string &filename, const std::string &content)
    {
        std::ofstream ofs(filename, std::ios::out | std::ios::binary | std::ios::trunc);
        if (ofs.is_open() == false)
        {
            ERROR("write file failed! {}", filename);
            return false;
        }
        ofs.write(content.c_str(), content.size());
        ofs.close();
        return true;
    }  

    /*
        字符串操作
    */
    size_t STRING::split(const std::string &str, const std::string &sep, std::vector<std::string> &parts)
    {
        size_t pos = 0, idx = 0;
        while(1) 
        {
            pos = str.find(sep, idx);
            if(pos == std::string::npos) {
                break;
            }
            if(pos == idx)
            {
                idx = pos + sep.size();
                continue;
            }
            parts.push_back(str.substr(idx, pos - idx));
            idx = pos + sep.size();
        }
        if(idx < str.size())
            parts.push_back(str.substr(idx));
        
        return parts.size();
    }

    /*
        随机数
    */
    std::string RANDOM::generateRandomString(size_t length, StrType type)
    {
        static const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        static const std::string numbers = "0123456789";
        static const std::string mixtype = characters + numbers;
        static std::atomic<uint16_t> seed(0);

        const std::string *ptr = nullptr;
        if(type == CHARACTER) {
            ptr = &characters;
        }
        else if(type == NUMBER) {
            ptr = &numbers;
        }
        else if(type == MIXTYPE) {
            ptr = &mixtype;
        }

        std::stringstream result; //  随机字符串
        std::random_device rd; //   用于获取随机数的种子
        std::mt19937 generator(rd()); // 随机数生成器

        for(size_t i = 0; i < length; i++) {
            int idx = generator() % ptr->size();
            result << ptr->operator[](idx);
        }

        result << std::setfill('0') << std::setw(5) << seed.fetch_add(1);
        return result.str();
    }
}
