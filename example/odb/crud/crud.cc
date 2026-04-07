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

void insert(std::unique_ptr<odb::mysql::database>&& handler)
{
    try
    {
        // 开启事务
        odb::mysql::transaction tx(handler->begin());
        // 获取数据库操作句柄
        auto& db = tx.database();
        // 插入数据
        Student s("李四");
        s.set_age(18);
        s.set_birth_date(boost::posix_time::time_from_string("2005-01-01 14:20:50"));

        db.persist(s);// 新增
        tx.commit();// 提交事务
    }
    catch(odb::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void query(std::unique_ptr<odb::mysql::database>&& handler)
{
    try
    {
        // 开启事务
        odb::mysql::transaction tx(handler->begin());
        // 获取数据库操作句柄
        auto &db = tx.database();
        // 查询数据
        using Query = odb::query<Student>;
        auto stu = db.query_one<Student>(Query::sno == 1);
        if(!stu)
        {   
            std::cout << "查询失败" << std::endl;
            return;
        }
        std::cout << "查询成功" << std::endl;
        std::cout << "sno: " << stu->sno() << std::endl;
        std::cout << "name: " << stu->name() << std::endl;
        if (stu->age())
            std::cout << "age: " << stu->age().get() << std::endl;
        if (stu->score())
            std::cout << "score: " << stu->score().get() << std::endl;
         if (stu->birth_date())
            std::cout << "birth_date: " << boost::posix_time::to_simple_string(stu->birth_date().get()) << std::endl;

        tx.commit(); // 提交事务
    }
    catch (odb::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}

void update(std::unique_ptr<odb::mysql::database>&& handler)
{
    try
    {
        // 开启事务
        odb::mysql::transaction tx(handler->begin());
        // 获取数据库操作句柄
        auto &db = tx.database();
        // 查询数据
        using Query = odb::query<Student>;
        auto stu = db.query_one<Student>(Query::sno == 1);
        if(!stu)
        {   
            std::cout << "查询失败" << std::endl;
            return;
        }
        // 先查询，再更新
        stu->set_score(99.99);
        db.update(stu);// 更新
        tx.commit(); // 提交事务
    }
    catch (odb::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}

void select_all(std::unique_ptr<odb::mysql::database>&& handler)
{
    try
    {
        // 开启事务
        odb::mysql::transaction tx(handler->begin());
        // 获取数据库操作句柄
        auto &db = tx.database();
        // 查询数据
        using Query = odb::query<Student>;
        using result = odb::result<Student>;
        result res(db.query<Student>());
        for (auto& stu : res)
        {
            std::cout << "sno: " << stu.sno() << std::endl;
            std::cout << "name: " << stu.name() << std::endl;
            if (stu.age())
                std::cout << "age: " << stu.age().get() << std::endl;
            if (stu.score())
                std::cout << "score: " << stu.score().get() << std::endl;
             if (stu.birth_date())
                std::cout << "birth_date: " << boost::posix_time::to_simple_string(stu.birth_date().get()) << std::endl;
        }
        tx.commit(); // 提交事务
    }
    catch (odb::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}

void remove(std::unique_ptr<odb::mysql::database>&& handler)
{
    try
    {
        // 开启事务
        odb::mysql::transaction tx(handler->begin());
        // 获取数据库操作句柄
        auto &db = tx.database();
        // 查询数据
        using Query = odb::query<Student>;
        auto stu = db.erase_query<Student>(Query::sno == 1);
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
    //insert(std::move(handler));
    // update(std::move(handler));
    //select_all(std::move(handler));
    remove(std::move(handler));
    return 0;
}