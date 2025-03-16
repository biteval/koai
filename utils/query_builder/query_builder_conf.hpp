#ifndef __KOAI_CASS_QUERY_BUILDER_CONF__
#define __KOAI_CASS_QUERY_BUILDER_CONF__

#include<vector>
#include <string>

namespace qry_builder_conf{
    struct CassQueryBuilderConf{
        std::string clusterName;
        std::string keySpace;
        std::string strategy;
        std::string replicationFactor;
        std::string tableName;
        std::vector<std::string> titles;
        std::vector<std::string> primaryKey;
        CassQueryBuilderConf()=default;
        CassQueryBuilderConf(const CassQueryBuilderConf&)=delete;
        CassQueryBuilderConf& operator=(const CassQueryBuilderConf&)=delete;
        CassQueryBuilderConf(CassQueryBuilderConf&&)noexcept=default;
        CassQueryBuilderConf& operator=(CassQueryBuilderConf&&)noexcept=default;
    };
}


#endif // ! __KOAI_CASS_QUERY_BUILDER_CONF__ INCLUDED