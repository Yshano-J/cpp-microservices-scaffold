#include "../../source/aries_elastic.h"
#include <iostream>

const std::string index_name = "student";
const std::string url = "http://elastic:123456@192.168.237.129:9200/";

void create(const AriesElastic::ESClient::ptr& client)
{
    AriesElastic::Index index(index_name);
    index.setTokenizer("ikmax");
    auto name = index.addFieldElem("name");
    name->type("text");
    name->boost(3.0);
    name->analyzer("ikmax");

    index.addFieldElem("phone")->type("keyword");
    index.addFieldElem("age")->type("integer");
    index.addFieldElem("skills")->type("text");
    index.addFieldElem("birth")->type("date");
    index.addFieldElem("birth")->index("false");
    
    auto res = client->create(index);
    if (res == false)
    {
        ERROR("create index failed");
    }
}
// 2. 新增数据
void insert(const AriesElastic::ESClient::ptr &client)
{
    // 构造Insert 对象
    AriesElastic::Insert inserter(index_name, "2");
    // 设置字段数据
    inserter.add("name", "wangwu");
    inserter.add("phone", "15566669999");
    inserter.add("age", 20);
    inserter.add("birth", "2018-08-19 22:22:33");
    inserter.append("skills", "Go");
    inserter.append("skills", "Rust");
    // 执⾏操作
    auto res = client->insert(inserter);
    if (res == false)
    {
        return;
    }
}
// 3. 更新数据
void update1(const AriesElastic::ESClient::ptr &client)
{
    // 构造Updater对象
    AriesElastic::Update updater(index_name, "1");
    // 添加更新字段数据
    updater.add("name", "李四");
    updater.add("phone", "15566668888");
    updater.add("age", 20);
    updater.append("skills", "Go");
    updater.append("skills", "Rust");
    // 执⾏操作
    auto res = client->update(updater);
    if (res == false)
    {
        return;
    }
}
// 该更新操作是通过全量新增的⽅式进⾏跟新，等价于删除元数据，插⼊⼼数据
void update2(const AriesElastic::ESClient::ptr &client)
{
    AriesElastic::Insert inserter(index_name, "1");
    inserter.add("name", "王五");
    inserter.add("phone", "15566669999");
    inserter.add("age", 18);
    inserter.add("birth", "2018-08-19 22:22:33");
    inserter.append("skills", "C++");
    inserter.append("skills", "Java");
    inserter.append("skills", "Python");
    auto res = client->insert(inserter);
    if (res == false)
    {
        return;
    }
}
// 4. 查询数据
void select1(const AriesElastic::ESClient::ptr &client)
{
    // 构造Searcher对象
    AriesElastic::Search searcher(index_name);
    // 设置查询⽅式
    auto query = searcher.addQuery();
    query->match("name")->setValue("李四");
    // searcher.from(0);
    // searcher.size(1);
    // 执⾏操作
    auto res = client->search(searcher);
    if (!res)
    {
        return;
    }
    // 打印序列化结果
    std::cout << *aries_util::JSON::serialize(*res) << std::endl;
}
void select2(const AriesElastic::ESClient::ptr &client)
{
    AriesElastic::Search searcher(index_name);
    auto query = searcher.addQuery();
    // auto must = query->must();
    // must->match("name")->setValue("zhangsan");
    auto should = query->should();
    should->terms("phone")->setValue("15566669999");
    should->terms("phone")->setValue("11111111111");
    // auto must_not = query->must_not();
    // must_not->match("name")->setValue("wangwu");
    auto res = client->search(searcher);
    if (!res)
    {
        return;
    }
    std::cout << *aries_util::JSON::serialize(*res) << std::endl;
}
// 5. 删除数据
void remove1(const AriesElastic::ESClient::ptr &client)
{
    // 构造Deleter对象，设置索引名称以及要删除的数据ID
    AriesElastic::Delete deleter(index_name, "1");
    auto res = client->remove(deleter);
    if (res == false)
    {
        return;
    }
}
// 6. 删除索引
void remove2(const AriesElastic::ESClient::ptr &client)
{
    auto res = client->remove(index_name);
    if (res == false)
    {
        return;
    }
}

int main()
{
    aries_log::global_logger_init();
    auto client = std::make_shared<AriesElastic::ESClient>(std::vector<std::string>{url});
    // create(client);
    // insert(client);
    // update2(client);
    // select2(client);
    // remove1(client);
    remove2(client);
    return 0;
}