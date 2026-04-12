#include "../../source/aries_mail.h"
#include "../../source/aries_log.h"
#include <memory>


int main()
{
    aries_log::global_logger_init();
    // 1. 构造邮件服务配置
    AriesCode::mail_settings settings = {
        .user = "15629267212@163.com",
        .password = "FYnFvceMrngzeXdQ",
        .url = "smtps://smtp.163.com:465",
        .from = "15629267212@163.com",
    };
    // 2. 实例化邮件客⼾端对象
    auto mail = std::make_unique<AriesCode::MailCodeClient>(settings);
    // 3. 发送邮件
    bool ret = mail->send("2779059669@qq.com", "195498");
    if (!ret)
    {
        WARNING("发送邮件失败！");
        return 1;
    }
    return 0;
}