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

void get_class_student(std::unique_ptr<odb::mysql::database> &&handler)
{
    try
    {
        // 开启事务
        odb::mysql::transaction tx(handler->begin());
        // 获取数据库操作句柄
        auto &db = tx.database();

        using Query = odb::query<ClassStudent>;
        using result = odb::result<ClassStudent>;
        result res(db.query<ClassStudent>(Query::Class::name == "一年级一班"));
        for(auto &it : res){
            if(it.stu){
                std::cout << it.stu->name() << std::endl;
            }
            if(it.cls){
                std::cout << it.cls->name() << std::endl;
            }
        }
        tx.commit();   // 提交事务
    }
    catch (odb::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}

void get_detail_student(std::unique_ptr<odb::mysql::database> &&handler)
{
    try
    {
        // 开启事务
        odb::mysql::transaction tx(handler->begin());
        // 获取数据库操作句柄
        auto &db = tx.database();

        using Query = odb::query<Student>;
        std::cout << "查询结果：" << std::endl;
        auto res = db.query_one<Student>(Query::name == "赵六");
        if(res->age())
            std::cout << res->age().get() << std::endl;
        if(res->score())
            std::cout << res->score().get() << std::endl;
        if(res->birth_date())
            std::cout << boost::posix_time::to_simple_string(res->birth_date().get()) << std::endl;

        tx.commit(); // 提交事务
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
    get_class_student(std::move(handler));
    get_detail_student(std::move(handler));
    return 0;
}