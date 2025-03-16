#include "database_controller.hpp"

void database_ctl::DataBaseController::setCredentials(std::string&& hosts, database::Credentials&&cred){
    db.setCredentials(std::move(hosts), std::move(cred));
    auto titles = cassQb.getTableTitles();
    db.setColumnsTitles(titles).conenct();
}


database_ctl::DataBaseController::DataBaseController(std::shared_ptr<logging::KoAiLogger>&logger){
    this->logger=logger;
    currMode = database_ctl::MODE::ADD_NEW;
}


void database_ctl::DataBaseController::setMode(database_ctl::MODE mode)const{
    currMode=mode;
}


database_ctl::MODE database_ctl::DataBaseController::getCurrMode()const{
   return currMode;
}


bool database_ctl::DataBaseController::isEqualRecord(nlohmann::json&firstRecord, nlohmann::json&secondRecord)const{
    if(firstRecord!=nullptr && secondRecord==nullptr){
        return false;// firstRecord is not added before to database so return true to add it.
    }
    if(!firstRecord.empty() && secondRecord.empty()){
        return false; // firstRecord is not added before to database so return true to add it.
    }
    if(firstRecord.empty()){
        return true;
    }
    if(firstRecord.empty() && secondRecord.empty()){
        return true;
    }
    using namespace database_titles;
    return firstRecord[FIRST_NAME_KEY] == secondRecord[FIRST_NAME_KEY] &&
    firstRecord[LAST_NAME_KEY] == secondRecord[LAST_NAME_KEY] &&
    firstRecord[DIVISION_KEY] == secondRecord[DIVISION_KEY] &&
    firstRecord[NICKNAME_KEY] == secondRecord[NICKNAME_KEY] &&
    firstRecord[STRIKING_ACCURACY_KEY] == secondRecord[STRIKING_ACCURACY_KEY] &&
    firstRecord[TAKEDOWN_ACCURACY_KEY] == secondRecord[TAKEDOWN_ACCURACY_KEY] &&
    firstRecord[SIG_STR_LANDED_PER_MIN_KEY] == secondRecord[SIG_STR_LANDED_PER_MIN_KEY] &&
    firstRecord[SIG_STR_ABSORBED_PER_MIN_KEY] == secondRecord[SIG_STR_ABSORBED_PER_MIN_KEY] &&
    firstRecord[TAKEDOWN_AVG_PER_15_MIN_KEY] == secondRecord[TAKEDOWN_AVG_PER_15_MIN_KEY] &&
    firstRecord[SIG_STR_DEFENSE_KEY] == secondRecord[SIG_STR_DEFENSE_KEY] &&
    firstRecord[KNOCKDOWN_AVG_KEY] == secondRecord[KNOCKDOWN_AVG_KEY] &&
    firstRecord[SUBMISSION_AVG_KEY] == secondRecord[SUBMISSION_AVG_KEY] &&
    firstRecord[TAKEDOWN_DEFENSE_KEY] == secondRecord[TAKEDOWN_DEFENSE_KEY] &&
    firstRecord[AVERAGE_FIGHT_TIME_AS_MINUTES_KEY] == secondRecord[AVERAGE_FIGHT_TIME_AS_MINUTES_KEY] &&
    firstRecord[SIG_STR_BY_POS_STANDING_KEY] == secondRecord[SIG_STR_BY_POS_STANDING_KEY] &&
    firstRecord[SIG_STR_BY_POS_CLINCH_KEY] == secondRecord[SIG_STR_BY_POS_CLINCH_KEY] &&
    firstRecord[SIG_STR_BY_POS_GROUND_KEY] == secondRecord[SIG_STR_BY_POS_GROUND_KEY] &&
    firstRecord[SIG_STR_BY_TARGET_HEAD_KEY] == secondRecord[SIG_STR_BY_TARGET_HEAD_KEY] &&
    firstRecord[SIG_STR_BY_TARGET_BODY_KEY] == secondRecord[SIG_STR_BY_TARGET_BODY_KEY] &&
    firstRecord[SIG_STR_BY_TARGET_LEG_KEY] == secondRecord[SIG_STR_BY_TARGET_LEG_KEY] &&
    firstRecord[WIN_BY_KO_TKO_KEY] == secondRecord[WIN_BY_KO_TKO_KEY] &&
    firstRecord[WIN_BY_DEC_KEY] == secondRecord[WIN_BY_DEC_KEY] &&
    firstRecord[WIN_BY_SUB_KEY] == secondRecord[WIN_BY_SUB_KEY] &&
    firstRecord[STATUS_KEY] == secondRecord[STATUS_KEY] &&
    firstRecord[FIGHTING_STYLE_KEY] == secondRecord[FIGHTING_STYLE_KEY] &&
    firstRecord[AGE_KEY] == secondRecord[AGE_KEY] &&
    firstRecord[HEIGHT_DB_KEY] == secondRecord[HEIGHT_DB_KEY] &&
    firstRecord[WEIGHT_DB_KEY] == secondRecord[WEIGHT_DB_KEY] &&
    firstRecord[GENDER_KEY] == secondRecord[GENDER_KEY] &&
    firstRecord[WEIGHT_CLASS_DB_KEY] == secondRecord[WEIGHT_CLASS_DB_KEY] &&
    firstRecord[IMAGE_URL_DB_KEY] == secondRecord[IMAGE_URL_DB_KEY] &&
    firstRecord[IMAGE_HEIGHT_DB_KEY] == secondRecord[IMAGE_HEIGHT_DB_KEY] &&
    firstRecord[IMAGE_WIDTH_DB_KEY] == secondRecord[IMAGE_WIDTH_DB_KEY];
}


void database_ctl::DataBaseController::changeClusterName(){
    auto qry = cassQb.getChangeClusterNameQuery();
    db.perform(qry);
}

void database_ctl::DataBaseController::createKeySpace(){
    auto qry = cassQb.getCreateKeySpaceQuery();
    db.perform(qry);
}

void database_ctl::DataBaseController::createTable(){
    auto qry = cassQb.getCreateTableQuery();
    db.perform(qry);
}

void database_ctl::DataBaseController::addRecord(nlohmann::json&&record)const{
    if(currMode==database_ctl::MODE::ADD_NEW || currMode==database_ctl::MODE::COMMING_FROM_UPDATE){
        /*
     set target column titles to retirieve
      auto columnTitles =cassQb.getCassQryBuilderTableTitles();
     db.setColumnsTitles(columnTitles);

    */
   auto currRecord = std::move(record);

   std::string fullName = std::string(currRecord[database_titles::FIRST_NAME_KEY]);
   fullName+=" ";
   fullName+=std::string(currRecord[database_titles::LAST_NAME_KEY]);

   if(currMode==database_ctl::MODE::COMMING_FROM_UPDATE){
    logger->add("updating ",fullName,"..");
    logger->log(logging::LogMode::RECENT);
    logger->clear(logging::LogMode::RECENT);
   }
    //build the inseret query 
    std::string insertCmd =cassQb.getInsertQuery(currRecord);
    
    if(insertCmd.empty()){
        //printf("empty query\n");
        logger->add(" failed, empty query\n");
        logger->log(logging::LogMode::RECENT);
        logger->clear(logging::LogMode::RECENT); 
        return;
      }
      //add the record to db.
      auto res = db.perform(insertCmd);
      
      if(res.empty() && currMode==database_ctl::MODE::COMMING_FROM_UPDATE){
        logger->add(" Ok.\n");
        logger->log(logging::LogMode::RECENT);
        logger->clear(logging::LogMode::RECENT); 
      }
   }
}


void database_ctl::DataBaseController::updateRecord(nlohmann::json&& record){
    if(currMode==database_ctl::MODE::UPDATE){
      auto currRecord = std::move(record);
      //retrieve the record from database
      std::string selectQuery=cassQb.getSelectQuery(currRecord);
      //printf("selectQuery = %s\n", selectQuery.c_str());
      nlohmann::json retrievedRecord = db.perform(selectQuery);
      
      if(!isEqualRecord(currRecord, retrievedRecord[0])){
        //remove the old record
        //printf("updating %s ..\n", record[FIRST_NAME_KEY].dump().c_str());
        std::string removeQuery=cassQb.getRemoveQuery(retrievedRecord[0]);
        //printf("removeQuery = %s\n", removeQuery.c_str());
        db.perform(removeQuery);
        
        //add the new record
        this->setMode(database_ctl::MODE::COMMING_FROM_UPDATE);
        addRecord(std::move(currRecord));
        this->setMode(database_ctl::MODE::UPDATE);
      }
      else{
        if(!retrievedRecord.empty() && !retrievedRecord[0].empty()){
            std::string fullName = std::string(retrievedRecord[0][database_titles::FIRST_NAME_KEY]);
            fullName+=" ";
            fullName+=std::string(retrievedRecord[0][database_titles::LAST_NAME_KEY]);
            logger->add("athlete ",fullName, " is up to date.\n");
            logger->log(logging::LogMode::RECENT);
            logger->clear(logging::LogMode::RECENT);
        }
        
      }
    }
}


nlohmann::json database_ctl::DataBaseController::search(std::string athleteName){
    if(currMode==database_ctl::MODE::RETRIEVE){
        std::string selectQry = cassQb.getSelectQuery(athleteName);
        //printf("selectQry=%s\n", selectQry.c_str());
        nlohmann::json retrievedRecord = db.perform(selectQry);
        return retrievedRecord;
    }
    //invalid mode
    return nlohmann::json{};
}


//search based on keys in the record using getUniqueSelectQuery
nlohmann::json database_ctl::DataBaseController::search(nlohmann::json&record){
    if(currMode==database_ctl::MODE::RETRIEVE){
        std::string selectQry = cassQb.getUniqueSelectQuery(record);
        //printf("database_controler.hpp selectQry=%s\n", selectQry.c_str());
        nlohmann::json retrievedRecord = db.perform(selectQry);
        return retrievedRecord;
    }
    //invalid mode
    return nlohmann::json{};
}

std::vector<std::string> database_ctl::DataBaseController::getCassQryBuilderTableTitles(){
  return cassQb.getTableTitles();
}
