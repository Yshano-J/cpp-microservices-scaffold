
#include "../../source/aries_log.h"
#include "../../source/util.h"
#include <iostream>
#include <vector>
#include <thread>
#include "../../source/aries_fastdfs.h"

std::string file_test()
{
    // 基于⽂件的上传下载测试
    //  1. 将⽂件上传到fastdfs服务器
    auto id = AriesFastdfs::FDFSClient::upload_from_file("./makefile");
    if (!id)
    {
        ERROR("上传⽂件失败!");
        abort();
    }
    // 2. 从fastdfs服务器下载到⽂件
    bool ret = AriesFastdfs::FDFSClient::download_to_file(*id, "./makefile.download1");
    if (!ret)
    {
        ERROR("下载⽂件失败!");
        abort();
    }
    // 3. 通过MD5sum命令⽐较⽂件是否⼀致 -- 由我们外部执⾏命令完成
    DBG("上传⽂件成功! file_id: {}", *id);
    return *id;
}

std::string buff_test()
{
    // 基于内存的上传下载测试
    //  1. 从⽂件中读取数据到内存
    std::string body;
    aries_util::FILE_UTIL::read("./makefile", body);
    // 2. 从内存上传到fastdfs服务器
    auto id = AriesFastdfs::FDFSClient::upload_from_buff(body);
    if (!id)
    {
        ERROR("上传⽂件失败!");
        abort();
    }
    // 3. 从fastdfs服务器下载到内存
    std::string buff;
    bool ret = AriesFastdfs::FDFSClient::download_to_buff(*id, buff);
    // 4. 将内存中的数据写⼊到⽂件
    aries_util::FILE_UTIL::write("./makefile.download2", buff);
    // 5. 通过MD5sum命令⽐较⽂件是否⼀致 -- 由我们外部执⾏命令完成
    DBG("上传⽂件成功! file_id: {}", *id);
    return *id;
}

void test_multi_thread(std::vector<std::string> &ids, std::mutex &mtx)
{
    // 多线程测试
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++)
    {
        threads.emplace_back([&]()
        {
            for (int j = 0;j < 5;j++) {
                auto id = AriesFastdfs::FDFSClient::upload_from_file("./makefile");
                if (!id)
                {
                    ERROR("上传⽂件失败!");
                    abort();
                }
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    ids.push_back(*id);
                }
                DBG("上传⽂件成功! file_id: {}", *id);
            }
        });
    }

    for (auto &t : threads)
    {
        t.join();
    }
}

void remove_test(const std::vector<std::string> &arr)
{
    // 删除测试
    for (auto &id : arr)
    {
        bool ret = AriesFastdfs::FDFSClient::remove(id);
        if (!ret)
        {
            ERROR("删除⽂件失败!");
            abort();
        }
        DBG("删除⽂件成功! file_id: {}", id);
    }
}

int main()
{
    aries_log::global_logger_init();
    AriesFastdfs::fdfs_settings settings = {
        .trackers = {"192.168.237.129:22122"},
    };
    AriesFastdfs::FDFSClient::init(settings);

    auto id1 = file_test();
    auto id2 = buff_test();
    std::vector<std::string> ids = {id1, id2};
    remove_test(ids);
    ids.clear();
    
    std::mutex mtx;
    test_multi_thread(ids, mtx);
    remove_test(ids);
    return 0;
}