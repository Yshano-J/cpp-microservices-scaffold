#pragma once
#include <string>
#include <cstddef> // std::size_t
#include <boost/date_time/posix_time/posix_time.hpp>
#include <odb/nullable.hxx>
#include <odb/core.hxx>

#pragma db object table("table_student")
class Student 
{
    public:
        Student(){};
        Student(const std::string& name) : _name(name){}
        int sno() const {return _sno;}
        void set_sno(int sno) {_sno = sno;}
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
        std::string _name;
        odb::nullable<int> _age;
        odb::nullable<double> _score;
        odb::nullable<boost::posix_time::ptime> _birth_date;
};