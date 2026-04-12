#pragma once
#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <memory>

namespace AriesCode
{
    struct mail_settings
    {   
        std::string user;
        std::string password;
        std::string url;
        std::string from;
    };

    class CodeClient
    {
        public:
            CodeClient() = default;
            virtual ~CodeClient() = default;
            virtual bool send(const std::string &to, const std::string &code) = 0;
    };

    class MailCodeClient : public CodeClient
    {
        public:
            using ptr = std::shared_ptr<MailCodeClient>;
            MailCodeClient(const mail_settings& settings);
            ~MailCodeClient();
            virtual bool send(const std::string &to, const std::string &code) override;

        private:
            std::stringstream CodeBody(const std::string &to, const std::string &code); // 构造验证正文
            static size_t callback(char *buffer, size_t size, size_t nitems, void *userdata);

        private:
            mail_settings _settings;
            std::string _title = "验证码";
    };
}
