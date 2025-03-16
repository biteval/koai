#ifndef __KOAI_DATABASE_CONTROLLER__
#define __KOAI_DATABASE_CONTROLLER__

/**
* @author Mohamed Naamane (mrnmnwork@gmail.com)
* @brief cassandra database controller, provides basic operations like add record or search for a record.
*/


#include "database_ctl_modes.hpp"
#include "../database/cassandra.hpp"
#include "../database/database_titles.hpp"
#include "../utils/query_builder/cass_query_builder.hpp"
#include "../loggers/koai_logger.hpp"


namespace database_ctl{
    class DataBaseController{
        mutable MODE currMode;
        mutable database_utils::CassQueryBuilder cassQb;
        database::Cassandra db;
        std::shared_ptr<logging::KoAiLogger>logger;
        public:
        DataBaseController()=default;
        DataBaseController(std::shared_ptr<logging::KoAiLogger>& logger);
        void setLogger(std::shared_ptr<logging::KoAiLogger>&logger){
          this->logger=logger;
        }
        void setCredentials(std::string&& hosts, database::Credentials&&cred);
        //move a configured cass query builder will be used to generate queries.
        void conf(auto&&configuredCassQb);
        //set the current database operation mode.
        void setMode(database_ctl::MODE mode)const;
        database_ctl::MODE getCurrMode()const;
        //compare 2 records by key val.
        bool isEqualRecord(nlohmann::json&firstRecord, nlohmann::json&secondRecord)const;
        //insert a new record to the database.
        void addRecord(nlohmann::json&& record)const;
        //update the given record
        void updateRecord(nlohmann::json&& record);
        //get the titles vector from the query builder.
        std::vector<std::string>getCassQryBuilderTableTitles();
        //search in the database by athlete name returns sugg in more than one athletes exists.
        nlohmann::json search(std::string athleteName);
        //search based on keys in the record getUniqueSelectQuery
        nlohmann::json search(nlohmann::json&record);
        void changeClusterName();
        void createKeySpace();
        void createTable();
        
        DataBaseController(const DataBaseController&)=delete;
        DataBaseController&operator=(const DataBaseController&)=delete;
        DataBaseController(DataBaseController&&)=delete;
        DataBaseController&operator=(DataBaseController&&)=delete;
        ~DataBaseController(){};
    };
}


void database_ctl::DataBaseController::conf(auto&&configuredCassQb){
    cassQb=std::forward<decltype(configuredCassQb)>(configuredCassQb);
}

#endif // ! __KOAI_DATABASE_CONTROLLER__ INCLUDED