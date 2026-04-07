#include "aries_odb.h"

namespace AriesOdb
{
    std::unique_ptr<odb::mysql::database> OdbFactory::create(const odb_settings &settings)
    {
        std::unique_ptr<odb::mysql::connection_factory> pool(new odb::mysql::connection_pool_factory(settings.connection_pool_size));

        return std::make_unique<odb::mysql::database>(settings.user, settings.password, settings.database, settings.host, settings.port, 
                                                    nullptr, settings.charset, 0, std::move(pool));
    }
}