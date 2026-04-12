#include "aries_mail.h"
#include "aries_log.h"

namespace AriesCode
{
    MailCodeClient::MailCodeClient(const mail_settings &settings) :_settings(settings) 
    {
        // 1. 初始化全局配置
        auto ret = curl_global_init(CURL_GLOBAL_DEFAULT);
        if (ret != CURLE_OK)
        {
            ERROR("curl_global_init failed: {}", curl_easy_strerror(ret));
        }
    }

    MailCodeClient::~MailCodeClient() 
    {
        // 释放全局配置
        curl_global_cleanup();
    }

    bool MailCodeClient::send(const std::string &to, const std::string &code)
    {
        // 2. 构造操作句柄
        auto curl = curl_easy_init();
        if (curl == nullptr)
        {
            ERROR("curl_easy_init failed");
            return false;
        }

        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 15L); // 连接超时时间
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L); // 数据传输超时时间

        // 3. 设置请求参数：url，username，password，body, from, to
        auto ret = curl_easy_setopt(curl, CURLOPT_URL, _settings.url.c_str());
        if (ret != CURLE_OK)
        {
            ERROR("curl_easy_setopt CURLOPT_URL failed: {}", curl_easy_strerror(ret));
            return false;
        }

        ret = curl_easy_setopt(curl, CURLOPT_USERNAME, _settings.user.c_str());
        if (ret != CURLE_OK)    
        {
            ERROR("curl_easy_setopt CURLOPT_USERNAME failed: {}", curl_easy_strerror(ret));
            return false;
        }

        ret = curl_easy_setopt(curl, CURLOPT_PASSWORD, _settings.password.c_str());
        if (ret != CURLE_OK)
        {
            ERROR("curl_easy_setopt CURLOPT_PASSWORD failed: {}", curl_easy_strerror(ret));
            return false;
        }

        ret = curl_easy_setopt(curl, CURLOPT_MAIL_FROM, _settings.from.c_str());
        if (ret != CURLE_OK)
        {
            ERROR("curl_easy_setopt CURLOPT_MAIL_FROM failed: {}", curl_easy_strerror(ret));
            return false;
        }

        struct curl_slist *cs = nullptr;
        cs = curl_slist_append(cs, to.c_str());
        ret = curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, cs);
        if (ret != CURLE_OK)
        {
            ERROR("curl_easy_setopt CURLOPT_MAIL_RCPT failed: {}", curl_easy_strerror(ret));
            return false;
        }

        std::stringstream body = CodeBody(to, code);
        ret = curl_easy_setopt(curl, CURLOPT_READDATA, (void*)&body);
        if (ret != CURLE_OK)
        {
            ERROR("curl_easy_setopt CURLOPT_READDATA failed: {}", curl_easy_strerror(ret));
            return false;
        }

        ret = curl_easy_setopt(curl, CURLOPT_READFUNCTION, &MailCodeClient::callback);
        if (ret != CURLE_OK)
        {
            ERROR("curl_easy_setopt CURLOPT_READFUNCTION failed: {}", curl_easy_strerror(ret));
            return false;
        }

        ret = curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        if (ret != CURLE_OK)
        {
            ERROR("curl_easy_setopt CURLOPT_UPLOAD failed: {}", curl_easy_strerror(ret));
            return false;
        }

        // 4. 执⾏请求
        ret = curl_easy_perform(curl);
        if (ret != CURLE_OK)
        {
            ERROR("curl_easy_perform failed:  {}", curl_easy_strerror(ret));
            return false;
        }
        // 释放链表
        curl_slist_free_all(cs);
        // 释放句柄
        curl_easy_cleanup(curl);
        return true;
    }

    std::stringstream MailCodeClient::CodeBody(const std::string &to, const std::string &code)
    {
        // 构造邮件正⽂，格式可不能错了，包含头部和内容两部分信息
        std::stringstream ss;
        ss << "From: " << _settings.from << "\r\n";
        ss << "To: " << to << "\r\n"; // 如果有多个收件⼈，则收件⼈之间以,间隔
        ss << "Subject: " << _title << "\r\n";
        ss << "MIME-Version: 1.0\r\n";
        ss << "Content-Type: text/html; charset=UTF-8\r\n";
        ss << "\r\n";
        ss << "<!DOCTYPE html>";
        ss << "<html lang=\"zh-CN\">";
        ss << "<head>";
        ss << "<meta charset=\"UTF-8\">";
        ss << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
        ss << "<title>验证码</title>";
        ss << "</head>";
        ss << "<body style=\"margin:0;padding:0;background-color:#f4f7fb;"
            "font-family:'Microsoft YaHei',Arial,sans-serif;color:#1f2937;\">";
        ss << "<table role=\"presentation\" cellpadding=\"0\" cellspacing=\"0\" width=\"100%\" "
            "style=\"background:linear-gradient(180deg,#eef4ff 0%,#f7f9fc 100%);padding:32px 16px;\">";
        ss << "<tr><td align=\"center\">";
        ss << "<table role=\"presentation\" cellpadding=\"0\" cellspacing=\"0\" width=\"600\" "
            "style=\"width:100%;max-width:600px;background:#ffffff;border-radius:18px;"
            "overflow:hidden;border:1px solid #e5e7eb;box-shadow:0 12px 32px rgba(15,23,42,0.08);\">";
        ss << "<tr><td style=\"padding:20px 28px;background:linear-gradient(135deg,#2563eb,#1d4ed8);"
            "color:#ffffff;\">";
        ss << "<div style=\"font-size:14px;opacity:0.92;letter-spacing:1px;\">SECURITY VERIFICATION</div>";
        ss << "<div style=\"margin-top:8px;font-size:28px;font-weight:700;\">验证码通知</div>";
        ss << "</td></tr>";
        ss << "<tr><td style=\"padding:32px 28px 12px 28px;\">";
        ss << "<div style=\"font-size:16px;line-height:28px;color:#374151;\">你好，</div>";
        ss << "<div style=\"margin-top:10px;font-size:16px;line-height:28px;color:#374151;\">"
            "你正在进行身份验证，本次操作的验证码如下：</div>";
        ss << "<div style=\"margin:28px 0 22px 0;text-align:center;\">";
        ss << "<div style=\"display:inline-block;padding:18px 30px;background:#eff6ff;"
            "border:1px dashed #93c5fd;border-radius:16px;\">";
        ss << "<span style=\"font-size:34px;font-weight:700;letter-spacing:8px;color:#1d4ed8;\">";
        ss << code;
        ss << "</span>";
        ss << "</div>";
        ss << "</div>";
        ss << "<div style=\"padding:16px 18px;background:#f9fafb;border-radius:12px;"
            "font-size:14px;line-height:24px;color:#4b5563;\">"
            "验证码将在 <span style=\"font-weight:700;color:#111827;\">5 分钟</span> 后失效。"
            "如非本人操作，请忽略本邮件，并注意保护账号安全。"
            "</div>";
        ss << "<div style=\"margin-top:24px;font-size:14px;line-height:24px;color:#6b7280;\">"
            "为了你的账号安全，请勿将验证码透露给任何人。"
            "</div>";
        ss << "</td></tr>";
        ss << "<tr><td style=\"padding:20px 28px 28px 28px;\">";
        ss << "<div style=\"height:1px;background:#e5e7eb;\"></div>";
        ss << "<div style=\"margin-top:18px;font-size:12px;line-height:22px;color:#9ca3af;\">"
            "此邮件由系统自动发送，请勿直接回复。"
            "</div>";
        ss << "</td></tr>";
        ss << "</table>";
        ss << "</td></tr>";
        ss << "</table>";
        ss << "</body></html>\r\n";
        return ss;
    }

    size_t MailCodeClient::callback(char *buffer, size_t size, size_t nitems, void *userdata)
    {
        std::stringstream *ss = (std::stringstream *)userdata;
        ss->read(buffer, size * nitems); // 将ss中的数据读取到buffer中，返回读取的字节数
        return ss->gcount();
    }
}
