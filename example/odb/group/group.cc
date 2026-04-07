#include <odb/database.hxx>
#include <odb/mysql/transaction.hxx>
#include <odb/mysql/database.hxx>

#include "student.h"
#include "student-odb.hxx"

const std::string HOST = "192.168.237.129";
const unsigned int PORT = 3306;
const std::string USER = "root";
const std::string PASSWORD = "123456";
const std::string DATABASE = "mytest";
const std::string CSET = "utf8mb4";

void get_group_info(std::unique_ptr<odb::mysql::database> &&handler)
{
    try
    {
        // 开启事务
        odb::mysql::transaction tx(handler->begin());

        tx.tracer(odb::stderr_full_tracer);

        // 获取数据库操作句柄
        auto &db = tx.database();

        using Query = odb::query<StudentGroup>;
        using result = odb::result<StudentGroup>;
        result res(db.query<StudentGroup>());
        for(auto &it : res){
            std::cout << "班级：" << it.class_id << " ";
            std::cout << "人数：" << it.count << " ";
            std::cout << "平均成绩：" << it.avg_score << std::endl;
        }
        tx.commit();   // 提交事务
    }
    catch (odb::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}


int main()
{
    // 连接池
    std::unique_ptr<odb::mysql::connection_factory> pool(new odb::mysql::connection_pool_factory(5));
    // 获取数据库操作句柄
    auto handler = std::make_unique<odb::mysql::database>(USER, PASSWORD, DATABASE, HOST, PORT, nullptr, CSET, 0, std::move(pool));
    get_group_info(std::move(handler));
    return 0;
}