
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
}
