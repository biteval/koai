#ifndef __KOAI_CASS_QUERY_BUILDER__
#define __KOAI_CASS_QUERY_BUILDER__

/**
* @author Mohamed Naamane (mrnmnwork@gmail.com)
* @brief generates basic cassandra queries for example insert a record, search for a record , create table queries
*/

#include "query_builder_conf.hpp"
#include <nlohmann/json.hpp>

namespace database_utils{
    enum ORDER{ASC,DESC};
    class CassQueryBuilder{
        qry_builder_conf::CassQueryBuilderConf queryConf;
        std::string currQuery;
        std::string getQualifiedTableName();//helper function to return the full qualified table name keyspace.tablename
        public:
        CassQueryBuilder();
        void setConf(qry_builder_conf::CassQueryBuilderConf &&queryConf);
        CassQueryBuilder &setClusteringOrderBy(std::string title, ORDER by);
        CassQueryBuilder &withTitles();
        CassQueryBuilder &withTablePrimaryKey();
        std::vector<std::string> getTableTitles();
        std::string getCreateTableQuery();
        std::string getCreateKeySpaceQuery();
        std::string getChangeClusterNameQuery();
        //all the primarykey values shoud be available as keys in the record.
        std::string getInsertQuery(nlohmann::json&record);
        //all the primarykey values shoud be available as keys in the record.
        std::string getSelectQuery(nlohmann::json&record);
        //select by the first key in the PrimaryKey.
        std::string getSelectQuery(std::string&key);
        //all the primarykey values shoud be available as keys in the record.
        std::string getRemoveQuery(nlohmann::json&record);
        //get select query based on the keys available ina  record
        std::string getUniqueSelectQuery(nlohmann::json&record);
        void clearQueries();
        CassQueryBuilder(const CassQueryBuilder&)=delete;
        CassQueryBuilder&operator=(const CassQueryBuilder&)=delete;
        CassQueryBuilder(CassQueryBuilder&&)noexcept=default;
        CassQueryBuilder&operator=(CassQueryBuilder&&)noexcept=default;
        ~CassQueryBuilder();
    };
}




#endif // ! __KOAI_CASS_QUERY_BUILDER__ INCLUDED