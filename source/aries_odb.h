#pragma once
#include <odb/database.hxx>
#include <odb/mysql/transaction.hxx>
#include <odb/mysql/database.hxx>

namespace AriesOdb
{
    struct odb_settings
    {
        std::string host;
        unsigned int port = 3306;
        std::string user = "root";
        std::string password;
        std::string database;
        std::string charset = "utf8mb4";
        int connection_pool_size = 3;
    };

    class OdbFactory
    {
        public:
            static std::unique_ptr<odb::mysql::database> create(const odb_settings &settings);
    };
}