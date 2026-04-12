#include "../../source/aries_elastic.h"
#include <iostream>

int main()
{
    AriesElastic::Index index("student");
    index.setTokenizer("ik_max");
    index.addFieldElem("name")->add("type", "text");
    index.addFieldElem("name")->add("analyzer", "ikmax");
    index.addFieldElem("name")->add("boost", 3.0);
    std::cout << index.to_string(true) << std::endl;

    AriesElastic::Insert insert("student", "1");
    insert.add("name", "zhangsan");
    insert.add("age", 18);
    insert.append("phone", "13800000000");
    insert.append("phone", "11111111111");
    insert.add("skills", "java");
    std::cout << insert.to_string(true) << std::endl;

    AriesElastic::Update update("student", "1");
    update.add("age", 20);
    std::cout << update.to_string(true) << std::endl;
    std::cout << update.getIndex() << " " << update.getId() << std::endl;

    AriesElastic::Delete remove("student", "1");
    std::cout << remove.to_string(true) << std::endl;
    std::cout << remove.getIndex() << " " << remove.getId() << std::endl;

    AriesElastic::Search search("student");  
    search.addQuery()->match("name")->setValue("zhangsan");
    std::cout << search.to_string(true) << std::endl;

        //     AriesElastic::Base base("ik_max");
        //     base.add("tokenizer", "ik_max_word").add("type", "custom");
        //     std::cout << base.to_string() << std::endl;
        // auto obj1 = std::make_shared<AriesElastic::Object>("ik_max1");
        // obj1->add("tokenizer1", "ik_max_word").add("type", "custom");

        // auto obj2 = std::make_shared<AriesElastic::Object>("ik_max2");
        // obj2->add("tokenizer2", "ik_max_word2").add("type", "custom");

        // auto analyzer = std::make_shared<AriesElastic::Analyzer>();
        // analyzer->addTokenizer("ik_max");
        // aries_log::global_logger_init();
        // auto index = std::make_shared<AriesElastic::Index>();
        // index->addSettings();
        // index->setTokenizer("ik_max");
        // auto properties = index->addMapping()->addProperties();
        // properties->addField("name")->add("type", "text");
        // properties->addField("name")->add("analyzer", "ikmax");
        // properties->addField("name")->add("boost", 3.0);

        // properties->addField("phone")->add("type", "keyword");
        // properties->addField("skills")->add("type", "text");
        // properties->addField("birth")->add("type", "date");
        // properties->addField("index")->add("type", "boolean");

        // auto query = std::make_shared<AriesElastic::QObject>("query");
        // query->term("name")->setValue("zhangsan");

        // query->terms("phone")->setValue("13800000000");
        // query->terms("phone")->setValue("11111111111");

        // query->match("phone")->setValue("13800000000");

        // query->multi_match()->appendField("name");
        // query->multi_match()->appendField("age");
        // query->multi_match()->setQuery("zhangsan");

        // query->range("age")->setRange(18, 25);

        // std::cout << query->to_string(true) << std::endl;

        // AriesElastic::QueryArray array("must");
        // array.term("name")->setValue("zhangsan");
        // array.terms("phone")->setValue("13800000000");
        // array.terms("phone")->setValue("11111111111");
        // array.match("phone")->setValue("13800000000");

        // array.multi_match()->appendField("name");
        // array.multi_match()->appendField("age");
        // array.multi_match()->setQuery("zhangsan");

        // array.range("age")->setRange(18, 25);

        // AriesElastic::Search search;
        // auto must = search.addQuery()->qbool()->must();
        // must->term("name")->setValue("zhangsan");
        // must->match("age")->setValue(18);

        // search.addQuery()->qbool()->minimum_should_match(1);

        // auto should = search.addQuery()->qbool()->should();
        // should->terms("phone")->setValue("13800000000");
        // should->terms("skills")->setValue("java");

        // std::cout << search.to_string(true) << std::endl;

        return 0;
}
