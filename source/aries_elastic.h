#pragma once 
#include "util.h"
#include <elasticlient/client.h>
#include <cpr/response.h>
#include <unordered_map>
#include <memory>

namespace AriesElastic
{
    class Base
    {
        public:
            using ptr = std::shared_ptr<Base>;
        public:
            Base(const std::string &key);
            // 添加字段: 基础类型，数组类型
            template <typename T>
            void add(const std::string &key, const T &val) {
                _val[key] = val;
            }

            template <typename T>
            void append(const std::string &key, const T &val) {
                _val[key].append(val);
            }

            const std::string &key() const;
            virtual const Json::Value val() const;
            virtual std::string to_string(bool styled = false) const;

        protected:
            std::string _key;
            Json::Value _val;
    };

    class Object : public Base
    {
        public:
            using ptr = std::shared_ptr<Object>;
            Object(const std::string &key);

            void addElem(const Base::ptr& elem);
            Object::ptr newObject(const std::string &key);

            template <typename R>
            std::shared_ptr<R> getElem(const std::string &key)
            {
                if (_subs.find(key) == _subs.end())
                {
                    return nullptr;
                }
                return std::dynamic_pointer_cast<R>(_subs[key]);
            }
            virtual const Json::Value val() const;
            virtual std::string to_string(bool styled = false) const override;

        private:
            std::unordered_map<std::string, Base::ptr> _subs;
    };

    class Array : public Base
    {
        public:
            using ptr = std::shared_ptr<Array>;
            Array(const std::string &key);
            void addElem(const Base::ptr& elem);
            Array::ptr newArray(const std::string &key);
            Object::ptr newObject(const std::string &key);
            virtual const Json::Value val() const;
            virtual std::string to_string(bool styled = false) const override;
        private:
            std::vector<Base::ptr> _subs;
    };

    // 分词器
    class Tokenizer : public Object
    {
        public:
            using ptr = std::shared_ptr<Tokenizer>;
            Tokenizer(const std::string &key);
            void ik_max_word(const std::string &tokenizer = "ik_max_word");
            void type(const std::string &type = "custom");
    };

    // 分词器父类
    class Analyzer : public Object
    {
        public:
            using ptr = std::shared_ptr<Analyzer>;
            Analyzer();
            Tokenizer::ptr addTokenizer(const std::string &tokenizer_name);
    };

    // 分词器相关父类
    class Analysis : public Object
    {
        public:
            using ptr = std::shared_ptr<Analysis>;
            Analysis();
            Analyzer::ptr addAnalyzer();
    };

    class Settings : public Object
    {
        public:
            using ptr = std::shared_ptr<Settings>;
            Settings();
            Analysis::ptr addAnalysis();
    };

    class Field : public Object
    {
        public:
            using ptr = std::shared_ptr<Field>;
            Field(const std::string &key);
            void type(const std::string &type );
            void boost(double boost = 1.0);
            void analyzer(const std::string &analyzer);
            void index(bool index = true);
    };

    class Properties : public Object
    {
        public:
            using ptr = std::shared_ptr<Properties>;
            Properties();
            Field::ptr addField(const std::string &key);
    };

    class Mapping : public Object
    {
        public:
            using ptr = std::shared_ptr<Mapping>;
            Mapping();
            Properties::ptr addProperties();
            void addDynamic(bool dynamic);
    };

    //  搜索条件
    class Term : public Object
    {
        public:
            using ptr = std::shared_ptr<Term>;
            Term(const std::string &field);
            template <typename T>
            void setValue(const T &value)
            {
                add(_field, value);
            }
        private:
            std::string _field;
    };
    class Terms : public Object
    {
        public:
            using ptr = std::shared_ptr<Terms>;
            Terms(const std::string &field);
            template <typename T>
            void setValue(const T &value)
            {
                append(_field, value);
            }

        private:
            std::string _field;
    };
    class Match : public Object
    {
        public:
            using ptr = std::shared_ptr<Match>;
            Match(const std::string &field);
            template <typename T>
            void setValue(const T &value)
            {
                add(_field, value);
            }

        private:
            std::string _field;
    };    
    class MultiMatch : public Object
    {
        public:
            using ptr = std::shared_ptr<MultiMatch>;
            MultiMatch();
            template <typename T>
            void setQuery(const T &value)
            {
                add("query", value);
            }
            void appendField(const std::string &field);
    };
    class Range : public Object
    {
        public:
            using ptr = std::shared_ptr<Range>;
            Range(const std::string &field);
            template <typename T>
            void setRange(const T &gt, const T &lt)
            {
                _field->add("gt", gt);
                _field->add("lt", lt);
            }

        private:
            Object::ptr _field;
    };

    class QObject : public Object
    {
        public:
            using ptr = std::shared_ptr<QObject>;
            QObject(const std::string &key);
            Term::ptr term(const std::string& field);
            Terms::ptr terms(const std::string& field);
            Match::ptr match(const std::string& field);
            MultiMatch::ptr multi_match();
            Range::ptr range(const std::string& field);
    };

    class QueryArray : public Array
    {
    public:
        using ptr = std::shared_ptr<QueryArray>;
        QueryArray(const std::string &key);
        Term::ptr term(const std::string &field);
        Terms::ptr terms(const std::string &field);
        Match::ptr match(const std::string &field);
        MultiMatch::ptr multi_match();
        Range::ptr range(const std::string &field);
    };

    class Must : public QueryArray
    {
        public:
            using ptr = std::shared_ptr<Must>;
            Must();
    };

    class MustNot : public QueryArray
    {
    public:
        using ptr = std::shared_ptr<MustNot>;
        MustNot();
    };

    class Should : public QueryArray
    {
    public:
        using ptr = std::shared_ptr<Should>;
        Should();
    };

    class QBool : public QObject
    {
    public:
        using ptr = std::shared_ptr<QBool>;
        QBool();
        Must::ptr must();
        MustNot::ptr must_not();
        Should::ptr should();
        void minimum_should_match(size_t count);
    };

    class Query : public QObject
    {
        public:
            using ptr = std::shared_ptr<Query>;
            Query();
            void match_all();
            QBool::ptr qbool();
            Must::ptr must();
            Should::ptr should();
            MustNot::ptr must_not();
    };

    class Request 
    {
        public:
            using ptr = std::shared_ptr<Request>;
            Request(const std::string &index, const std::string &type, const std::string &method, const std::string &id);
            const std::string& getIndex() const;
            const std::string& getType() const;
            const std::string& getMethod() const;
            const std::string& getId() const;
            void setIndex(const std::string &index);
            void setType(const std::string &type);
            void setMethod(const std::string &method);
            void setId(const std::string &id);
        private:
            std::string _index;
            std::string _type;
            std::string _method;
            std::string _id;
    };

    class Index : public Object, public Request
    {
        public:
            using ptr = std::shared_ptr<Index>;
            Index(const std::string &index);
            Settings::ptr addSettings();
            Mapping::ptr addMapping();
            Tokenizer::ptr setTokenizer(const std::string &tokenizer_name);
            Field::ptr addFieldElem(const std::string &key);
    };

    class Insert : public Object, public Request
    {
        public:
            using ptr = std::shared_ptr<Insert>;
            Insert(const std::string &index, const std::string &id);
    };
        
    class Update : public Object, public Request
    {
        public:
            using ptr = std::shared_ptr<Update>;
            Update(const std::string &index, const std::string &id);
            Object::ptr addDoc();
            template <typename T>
            void add(const std::string& key, const T& value)
            {
                addDoc()->add(key, value);
            }
            template <typename T>
            void append(const std::string &key, const T &value)
            {
                addDoc()->append(key, value);
            }
    };

    class Delete : public Object, public Request
    {
        public:
            using ptr = std::shared_ptr<Delete>;
            Delete(const std::string &index, const std::string &id);
    };

    class Search : public Object, public Request
    {
        public:
            using ptr = std::shared_ptr<Search>;
            Search(const std::string &index);
            Query::ptr addQuery();
            void size(size_t count);
            void from(size_t offset);
    };

    class BaseClient
    {
        public:
            using ptr = std::shared_ptr<BaseClient>;
            BaseClient() = default;
            virtual bool create(const Index& idx) = 0;
            virtual bool insert(const Insert& ins) = 0;
            virtual bool update(const Update& up) = 0;
            virtual bool remove(const Delete& del) = 0;
            virtual bool remove(const std::string& index) = 0;
            virtual std::optional<Json::Value> search(const Search &srch) = 0;
    };

    class ESClient : public BaseClient
    { 
        public:
            using ptr = std::shared_ptr<ESClient>;
            ESClient(const std::vector<std::string>& host);
            bool create(const Index& idx) override;
            bool insert(const Insert& ins) override;
            bool update(const Update& up) override;
            bool remove(const Delete& del) override;
            bool remove(const std::string& index) override;
            std::optional<Json::Value> search(const Search &srch) override;

        private:
            std::shared_ptr<elasticlient::Client> _client;
    };
}