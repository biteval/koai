#ifndef __KOAI_UFC_DATABASE_CONTROLLER_CONF__
#define __KOAI_UFC_DATABASE_CONTROLLER_CONF__

/**
* @brief a class to config cassandra database controller with info like Cluster, keyspace , table name, for ufc database
*/


#include "../database/database_titles.hpp"
#include "../utils/query_builder/cass_query_builder.hpp"
#include "controller_configurer.hpp"

namespace database_ctl{
 class UfcDataBaseControllerConf : public ControllerConfigurer<UfcDataBaseControllerConf>{
    public:
    UfcDataBaseControllerConf(){};
    UfcDataBaseControllerConf(const UfcDataBaseControllerConf&)=delete;
    UfcDataBaseControllerConf&operator=(const UfcDataBaseControllerConf&)=delete;
    UfcDataBaseControllerConf(UfcDataBaseControllerConf&&)=delete;
    UfcDataBaseControllerConf&operator=(UfcDataBaseControllerConf&&)=delete;
    //configure the database controller.
    void configure(auto&dbController);
    ~UfcDataBaseControllerConf(){};
 };
}


void database_ctl::UfcDataBaseControllerConf::configure(auto&dbController){
    using namespace database_titles;
    std::vector<std::string> titles{
        FIRST_NAME_KEY,
        LAST_NAME_KEY,
        DIVISION_KEY,
        NICKNAME_KEY,
        STRIKING_ACCURACY_KEY,
        TAKEDOWN_ACCURACY_KEY,
        SIG_STR_LANDED_PER_MIN_KEY,
        SIG_STR_ABSORBED_PER_MIN_KEY,
        TAKEDOWN_AVG_PER_15_MIN_KEY,
        SIG_STR_DEFENSE_KEY,
        KNOCKDOWN_AVG_KEY,
        SUBMISSION_AVG_KEY,
        TAKEDOWN_DEFENSE_KEY,
        AVERAGE_FIGHT_TIME_AS_MINUTES_KEY,
        SIG_STR_BY_POS_STANDING_KEY,
        SIG_STR_BY_POS_CLINCH_KEY,
        SIG_STR_BY_POS_GROUND_KEY,
        SIG_STR_BY_TARGET_HEAD_KEY,
        SIG_STR_BY_TARGET_BODY_KEY,
        SIG_STR_BY_TARGET_LEG_KEY,
        WIN_BY_KO_TKO_KEY,
        WIN_BY_DEC_KEY,
        WIN_BY_SUB_KEY,
        STATUS_KEY,
        FIGHTING_STYLE_KEY,
        AGE_KEY,
        HEIGHT_DB_KEY,
        WEIGHT_DB_KEY,
        GENDER_KEY,
        WEIGHT_CLASS_DB_KEY,
        IMAGE_URL_DB_KEY,
        IMAGE_WIDTH_DB_KEY,
        IMAGE_HEIGHT_DB_KEY,
    };
    
    std::vector<std::string> primaryKey{FIRST_NAME_KEY, GENDER_KEY, LAST_NAME_KEY, STATUS_KEY, NICKNAME_KEY,AGE_KEY,DIVISION_KEY,FIGHTING_STYLE_KEY,IMAGE_URL_DB_KEY};
       qry_builder_conf::CassQueryBuilderConf conf;
       conf.clusterName="Ufc Cluster";
       conf.keySpace="ufc";
       conf.strategy="SimpleStrategy";
       conf.tableName="athletes";
       conf.replicationFactor="1";
       conf.titles=std::move(titles);
       conf.primaryKey=std::move(primaryKey);
       database_utils::CassQueryBuilder qbuilder;
       //move the conf struct to the cass query builder obj.
       qbuilder.setConf(std::move(conf));
       //feeding qbuilder with other basic info required to generate queries
       qbuilder.withTitles().withTablePrimaryKey().setClusteringOrderBy(GENDER_KEY,database_utils::ORDER::ASC).
       setClusteringOrderBy(LAST_NAME_KEY,database_utils::ORDER::ASC).
       setClusteringOrderBy(STATUS_KEY, database_utils::ORDER::ASC).
       setClusteringOrderBy(NICKNAME_KEY, database_utils::ORDER::ASC).
       setClusteringOrderBy(AGE_KEY, database_utils::ORDER::ASC).
       setClusteringOrderBy(DIVISION_KEY, database_utils::ORDER::ASC).
       setClusteringOrderBy(FIGHTING_STYLE_KEY, database_utils::ORDER::ASC);
       //move the feeded qbuilder to dbController
       dbController.conf(std::move(qbuilder)); //dbController is a DataBaseController object
}

#endif //! __KOAI_UFC_DATABASE_CONTROLLER_CONF__ INCLUDED