#include "aries_elastic.h"
#include <iostream>
namespace AriesElastic
{
    Base::Base(const std::string &key) : _key(key) {}

    const std::string &Base::key() const { return _key; }
    const Json::Value Base::val() const { return _val; }

    std::string Base::to_string(bool styled) const
    {
        Json::Value root;
        root[_key] = _val;
        return *aries_util::JSON::serialize(root, styled);
    }

    Object::Object(const std::string &key) : Base(key) {}
    void Object::addElem(const Base::ptr &elem) { 
        _subs[elem->key()] = elem;
    }

    /*
        it.second 虽然类型是 Base::ptr，但因为 val() 是虚函数，
        所以会动态调用到实际对象的 val()

        于是：
            index    去取 settings->val()
            settings 去取 analysis->val()
            analysis 去取 analyzer->val()
            analyzer 去取 tokenizer->val()
            tokenizer 因为没有子节点，Object::val() 最终返回它自己的 _val
    */
    const Json::Value Object::val() const {
        Json::Value root = _val;
        for (auto &it : _subs)
        {
            root[it.first] = it.second->val();
        }
        return root; 
    }
    std::string Object::to_string(bool styled) const {
        Json::Value root = _val;
        for (auto &it : _subs)
        {
            root[it.first] = it.second->val();
        }
        return *aries_util::JSON::serialize(root, styled);
    }
    Object::ptr Object::newObject(const std::string &key) {
        Object::ptr obj = getElem<Object>(key);
        if(obj)
            return obj;
        obj = std::make_shared<AriesElastic::Object>(key);
        addElem(obj);
        return obj;
    }

    Array::Array(const std::string &key) : Base(key) {}
    void Array::addElem(const Base::ptr &elem) {
        _subs.push_back(elem);
    }
    Array::ptr Array::newArray(const std::string &key) {
        auto arr = std::make_shared<Array>(key);
        addElem(arr);
        return arr;
    }
    Object::ptr Array::newObject(const std::string &key) {
        auto obj = std::make_shared<Object>(key);
        addElem(obj);
        return obj;
    }
    const Json::Value Array::val() const {
        Json::Value root = _val;
        for (auto &it : _subs) {
            root.append(it->val());
        }
        return root;
    }
    std::string Array::to_string(bool styled) const {
        Json::Value root = _val;
        for (auto &it : _subs)
        {
            root.append(it->val());
        }
        return *aries_util::JSON::serialize(root, styled);
    }


    Tokenizer::Tokenizer(const std::string &key) : Object(key)
    {
        ik_max_word();
        type();
    }
    void Tokenizer::ik_max_word(const std::string &tokenizer)
    {
        add("tokenizer", tokenizer);
    }
    void Tokenizer::type(const std::string &type)
    {
        add("type", type);
    }

    Analyzer::Analyzer() : Object("analyzer") {}
    Tokenizer::ptr Analyzer::addTokenizer(const std::string &tokenizer_name) {
        Tokenizer::ptr tokenizer = getElem<Tokenizer>(tokenizer_name);
        if (tokenizer)
        {
            return tokenizer;
        }
        tokenizer = std::make_shared<AriesElastic::Tokenizer>(tokenizer_name);
        addElem(tokenizer);
        return tokenizer;
    }

    Analysis::Analysis() : Object("analysis") {}
    Analyzer::ptr Analysis::addAnalyzer() {
        Analyzer::ptr analyzer = getElem<Analyzer>("analyzer");
        if (analyzer)
        {
            return analyzer;
        }
        analyzer = std::make_shared<AriesElastic::Analyzer>();
        addElem(analyzer);
        return analyzer;
    }

    Settings::Settings() : Object("settings") {}
    Analysis::ptr Settings::addAnalysis() {
        Analysis::ptr analysis = getElem<Analysis>("analysis");
        if (analysis) {
            return analysis;
        }
        analysis = std::make_shared<AriesElastic::Analysis>();
        addElem(analysis);
        return analysis;
    }

    Field::Field(const std::string &key) : Object(key) {}
    void Field::type(const std::string &type) {
        add("type", type);
    }
    void Field::boost(double boost) {
        add("boost", boost);
    }
    void Field::analyzer(const std::string &analyzer) {
        add("analyzer", analyzer);
    }
    void Field::index(bool index) {
        add("index", index);
    }

    Properties::Properties() : Object("properties") {}
    Field::ptr Properties::addField(const std::string &key) {
        Field::ptr field = getElem<Field>(key);
        if (field) {
            return field;
        }
        field = std::make_shared<AriesElastic::Field>(key);
        addElem(field);
        return field;
    }

    Mapping::Mapping() : Object("mapping") {
        addDynamic(false);
    }
    Properties::ptr Mapping::addProperties() {
        Properties::ptr properties = getElem<Properties>("properties");
        if (properties) {
            return properties;
        }
        properties = std::make_shared<AriesElastic::Properties>();
        addElem(properties);
        return properties;
    }
    void Mapping::addDynamic(bool dynamic) {
        add("dynamic", dynamic);
    }

    QObject::QObject(const std::string &key) : Object(key) {}
    Term::Term(const std::string &field) : Object("term") , _field(field) {}
    Terms::Terms(const std::string &field) : Object("terms") , _field(field) {}
    Match::Match(const std::string &field) : Object("match") , _field(field) {}
    MultiMatch::MultiMatch() : Object("multi_match") {}
    void MultiMatch::appendField(const std::string &field) { append("fields", field); }
    Range::Range(const std::string &field) : Object("range") {
        _field = std::make_shared<AriesElastic::Object>(field);
        addElem(_field);
    }

    Term::ptr QObject::term(const std::string& field) {
        Term::ptr term = getElem<Term>("term");
        if (term)
        {
            return term;
        }
        term = std::make_shared<AriesElastic::Term>(field);
        addElem(term);
        return term;
    }

    Terms::ptr QObject::terms(const std::string &field)
    {
        Terms::ptr terms = getElem<Terms>("terms");
        if (terms)
        {
            return terms;
        }
        terms = std::make_shared<AriesElastic::Terms>(field);
        addElem(terms);
        return terms;
    }
    Match::ptr QObject::match(const std::string &field)
    {
        Match::ptr match = getElem<Match>("match");
        if (match)
        {
            return match;
        }
        match = std::make_shared<AriesElastic::Match>(field);
        addElem(match);
        return match;
    }
    MultiMatch::ptr QObject::multi_match() {
        MultiMatch::ptr multi_match = getElem<MultiMatch>("multi_match");
        if (multi_match)
        {
            return multi_match;
        }
        multi_match = std::make_shared<AriesElastic::MultiMatch>();
        addElem(multi_match);
        return multi_match;
    }
    Range::ptr QObject::range(const std::string& field) {
        Range::ptr range = getElem<Range>("range");
        if (range)
        {
            return range;
        }
        range = std::make_shared<AriesElastic::Range>(field);
        addElem(range);
        return range;
    }

    QueryArray::QueryArray(const std::string &key) : Array(key) {}
    Term::ptr QueryArray::term(const std::string &field) {
        auto obj = newObject("");
        auto term = std::make_shared<AriesElastic::Term>(field);
        obj->addElem(term);
        return term;
    }
    Terms::ptr QueryArray::terms(const std::string &field) {
        auto obj = newObject("");
        auto terms = std::make_shared<AriesElastic::Terms>(field);
        obj->addElem(terms);
        return terms;
    }
    Match::ptr QueryArray::match(const std::string &field) {
        auto obj = newObject("");
        auto match = std::make_shared<AriesElastic::Match>(field);
        obj->addElem(match);
        return match;
    }
    MultiMatch::ptr QueryArray::multi_match() {
        auto obj = newObject("");
        auto multi_match = std::make_shared<AriesElastic::MultiMatch>();
        obj->addElem(multi_match);
        return multi_match;
    }
    Range::ptr QueryArray::range(const std::string &field) {
        auto obj = newObject("");
        auto range = std::make_shared<AriesElastic::Range>(field);
        obj->addElem(range);
        return range;
    }

    QBool::QBool() : QObject("bool") {}
    Must::Must() : QueryArray("must") {}
    MustNot::MustNot() : QueryArray("must_not") {}  
    Should::Should() : QueryArray("should") {}

    Must::ptr QBool::must() {
        Must::ptr must = getElem<Must>("must");
        if (must)
        {
            return must;
        }
        must = std::make_shared<AriesElastic::Must>();
        addElem(must);
        return must;
    }
    MustNot::ptr QBool::must_not()
    {
        MustNot::ptr must_not = getElem<MustNot>("must_not");
        if (must_not)
        {
            return must_not;
        }
        must_not = std::make_shared<AriesElastic::MustNot>();
        addElem(must_not);
        return must_not;
    }
    Should::ptr QBool::should() {
        Should::ptr should = getElem<Should>("should");
        if (should)
        {
            return should;
        }
        should = std::make_shared<AriesElastic::Should>();
        addElem(should);
        return should;
    }

    void QBool::minimum_should_match(size_t count) {
        add("minimum_should_match", count);
    }

    QBool::ptr Query::qbool() {
        QBool::ptr qbool = getElem<QBool>("bool");
        if (qbool)
        {
            return qbool;
        }
        qbool = std::make_shared<AriesElastic::QBool>();
        addElem(qbool);
        return qbool;
    }

    Must::ptr Query::must()
    {
        return qbool()->must();
    }
    Should::ptr Query::should()
    {
        return qbool()->should();
    }
    MustNot::ptr Query::must_not()
    {
        return qbool()->must_not();
    }

    Query::Query() : QObject("query") {}
    void Query::match_all() {
        add("match_all", Json::Value(Json::ValueType::objectValue));
    }

    Request::Request(const std::string &index, const std::string &type, const std::string &method, const std::string &id)
                    : _index(index), _type(type), _method(method), _id(id) {}
    const std::string &Request::getIndex() const { return _index; }
    const std::string &Request::getType() const { return _type; }
    const std::string &Request::getMethod() const { return _method; }
    const std::string &Request::getId() const { return _id; }
    void Request::setIndex(const std::string &index) { _index = index; }
    void Request::setType(const std::string &type) { _type = type; }
    void Request::setMethod(const std::string &method) { _method = method; }
    void Request::setId(const std::string &id) { _id = id; }

    Settings::ptr Index::addSettings()
    {
        Settings::ptr settings = getElem<Settings>("settings");
        if (settings)
        {
            return settings;
        }
        settings = std::make_shared<AriesElastic::Settings>();
        addElem(settings);
        return settings;
    }

    Mapping::ptr Index::addMapping()
    {
        Mapping::ptr mapping = getElem<Mapping>("mapping");
        if (mapping)
        {
            return mapping;
        }
        mapping = std::make_shared<AriesElastic::Mapping>();
        addElem(mapping);
        return mapping;
    }

    Index::Index(const std::string &index) : Object(index) , Request(index, "_doc", "", index)  {}
    Tokenizer::ptr Index::setTokenizer(const std::string &tokenizer_name) {
        return addSettings()->addAnalysis()->addAnalyzer()->addTokenizer(tokenizer_name);
    }
    Field::ptr Index::addFieldElem(const std::string &key) {
        return addMapping()->addProperties()->addField(key);
    }

    Insert::Insert(const std::string &index, const std::string &id) 
        : Object("") , Request(index, "_doc", "_insert", id)  {}

    Update::Update(const std::string &index, const std::string &id)
        : Object(""), Request(index, "_doc", "_update", id) {}

    Delete::Delete(const std::string &index, const std::string &id) 
        : Object("") , Request(index, "_doc", "_delete", id)  {}

    Search::Search(const std::string &index) : Object(""), Request(index, "_doc", "_search", "")  {}
    Query::ptr Search::addQuery() {
        Query::ptr query = getElem<Query>("query");
        if (query)
        {
            return query;
        }
        query = std::make_shared<AriesElastic::Query>();
        addElem(query);
        return query;
    }
    void Search::size(size_t count)
    {
        add("size", count);
    }
    void Search::from(size_t offset)
    {
        add("from", offset);
    }

    Object::ptr Update::addDoc() {
        Object::ptr doc = getElem<Object>("doc");
        if (doc)
        {
            return doc;
        }
        doc = std::make_shared<AriesElastic::Object>("doc");
        addElem(doc);
        return doc;
    }

    ESClient::ESClient(const std::vector<std::string> &host) : _client(std::make_shared<elasticlient::Client>(host)) {}
    bool ESClient::create(const Index &idx) {
        std::string index_name = idx.getIndex();
        std::string type_name = idx.getType();
        std::string method = idx.getMethod();
        std::string doc_id = idx.getId();
        std::string body = idx.to_string();
        auto resp = _client->index(index_name, type_name, doc_id, body);
        if (resp.status_code < 200 || resp.status_code >= 300) {
            ERROR("create index failed: {}/{}/{} - {}", index_name, type_name, doc_id, resp.text);
            return false;
        }
        return true;
    }
    bool ESClient::insert(const Insert &ins) {
        std::string index_name = ins.getIndex();
        std::string type_name = ins.getType();
        std::string method = ins.getMethod();
        std::string doc_id = ins.getId();
        std::string body = ins.to_string();
        auto resp = _client->index(index_name, type_name, doc_id, body);
        if (resp.status_code < 200 || resp.status_code >= 300)
        {
            ERROR("insert data failed: {}/{}/{} - {}", index_name, type_name, doc_id, resp.text);
            return false;
        }
        return true;
    }
    bool ESClient::update(const Update &up) {
        std::string index_name = up.getIndex();
        std::string type_name = up.getType();
        std::string doc_id = up.getId();
        std::string body = up.to_string();
        std::string path = index_name + "/_update/" + doc_id;
        auto resp = _client->performRequest(elasticlient::Client::HTTPMethod::POST, path, body);
        if (resp.status_code < 200 || resp.status_code >= 300)
        {
            ERROR("update data failed: {}/{}/{} - {}", index_name, type_name, doc_id, resp.text);
            return false;
        }
        return true;
    }
    bool ESClient::remove(const Delete &del) {
        std::string index_name = del.getIndex();
        std::string type_name = del.getType();
        std::string doc_id = del.getId();
        auto resp = _client->remove(index_name, type_name, doc_id, "");
        if (resp.status_code < 200 || resp.status_code >= 300)
        {
            ERROR("remove data failed: {}/{}/{} - {}", index_name, type_name, doc_id, resp.text);
            return false;
        }
        return true;
    }
    bool ESClient::remove(const std::string &index) {
        auto resp = _client->performRequest(elasticlient::Client::HTTPMethod::DELETE, index, "");
        if (resp.status_code < 200 || resp.status_code >= 300)
        {
            ERROR("remove index failed: {} - {}", index, resp.text);
            return false;
        }
        return true;
    }
    std::optional<Json::Value> ESClient::search(const Search &srch) {
        std::string index_name = srch.getIndex();
        std::string type_name = srch.getType();
        std::string body = srch.to_string();
        auto resp = _client->search(index_name, type_name, body);
        if (resp.status_code < 200 || resp.status_code >= 300)
        {
            ERROR("search data failed: {}/{} - {}", index_name, type_name, resp.text);
            return std::nullopt;
        }
        auto resp_json = aries_util::JSON::deserialize(resp.text);
        if(!resp_json) {
            ERROR("search data failed: {}/{} - {}", index_name, type_name, resp.text);
            return std::nullopt;
        }
        if(resp_json->isNull() || (*resp_json)["hits"].isNull() || (*resp_json)["hits"]["total"].isNull()) {
            ERROR("search data failed: {}/{} - {}", index_name, type_name, resp.text);
            return std::nullopt;
        }
        Json::Value ret;
        int sz = (*resp_json)["hits"]["hits"].size();
        for(int i = 0; i < sz; i++) {
            ret.append((*resp_json)["hits"]["hits"][i]["_source"]);
        }
        return ret;
    }
}
