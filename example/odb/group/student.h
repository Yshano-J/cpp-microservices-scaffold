#pragma once
#include <memory>
#include <string>
#include <cstddef> // std::size_t
#include <boost/date_time/posix_time/posix_time.hpp>
#include <odb/nullable.hxx>
#include <odb/core.hxx>

#pragma db object table("student")
class Student 
{
    public:
        Student() = default;
        Student(const std::string& name) : _name(name){}
        int sno() const {return _sno;}
        void set_sno(int sno) {_sno = sno;}
        int class_id() const {return _class_id;}
        void set_class_id(int class_id) {_class_id = class_id;}
        std::string name() const {return _name;}
        void set_name(const std::string& name) {_name = name;}
        odb::nullable<int> age() const {return _age;}
        void set_age(const odb::nullable<int>& age) {_age = age;}
        odb::nullable<double> score() const {return _score;}
        void set_score(const odb::nullable<double>& score) {_score = score;}
        odb::nullable<boost::posix_time::ptime> birth_date() const {return _birth_date;}
        void set_birth_date(const odb::nullable<boost::posix_time::ptime>& birth_date) {_birth_date = birth_date;}

    private:
        friend class odb::access;

        #pragma db id auto
        int _sno;
        #pragma db index
        int _class_id;
        std::string _name;
        odb::nullable<int> _age;
        odb::nullable<double> _score;
        odb::nullable<boost::posix_time::ptime> _birth_date;
};

#pragma db object table("class_info")
class Class 
{
    public:
        Class() = default;
        Class(const std::string &name) : _name(name) {}
        int id() const {return _id;}
        void set_id(int id) {_id = id;}
        std::string name() const {return _name;}
        void set_name(const std::string& name) {_name = name;}

    private:
        friend class odb::access;
        
        #pragma db id auto
        int _id = 0;
        #pragma db type("varchar(32)")
        std::string _name;
};

#pragma db view object(Student) \
                query((?) + "group by class_id")
struct StudentGroup
{
    #pragma db column("class_id")
    int class_id;
    #pragma db column("count(" + Student::_class_id + ")")
    int count;
    #pragma db column("avg(" + Student::_score + ")")
    double avg_score;
};
