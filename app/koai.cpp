#include "koai.hpp"

koai::KoAi::KoAi(){
    initDataProvider();
}

void koai::KoAi::initDataProvider(){
    database::Credentials cred;
    cred.username = std::getenv(exported::CASS_USERNAME);
    cred.password = std::getenv(exported::CASS_PASS);
    if(cred.username==nullptr || cred.password==nullptr){
        auto logger = dataProvider.getLogger();
        logger->add("KoAi error : CASS_USERNAME | CASS_PASS env varibales not exported.\n");
        logger->log();
        logger->clear();
    }
    std::string hosts ="127.0.0.1";
    dataProvider.configureDbController(ufcCtlConfigurer).
    setCredentials(std::move(hosts), std::move(cred));
}


nlohmann::json koai::KoAi::getFighterBasicDetails(nlohmann::json&records){
    if(records==nullptr){
     return {};
    }
    constexpr size_t BASIC_REC_SIZE = 5;
    nlohmann::json basicDetails = nlohmann::json::array();
    for(size_t i =0; i<records.size();++i){
        if(records[i].size()==BASIC_REC_SIZE){
            //records comes from live online search with basic details
            return records;
        }
        nlohmann::json subRec{};
        subRec[database_titles::FIRST_NAME_KEY] = records[i][database_titles::FIRST_NAME_KEY];
        subRec[database_titles::LAST_NAME_KEY] = records[i][database_titles::LAST_NAME_KEY];
        subRec[database_titles::NICKNAME_KEY] = records[i][database_titles::NICKNAME_KEY];
        subRec[database_titles::IMAGE_URL_DB_KEY] = records[i][database_titles::IMAGE_URL_DB_KEY];
        subRec[database_titles::GENDER_KEY] = records[i][database_titles::GENDER_KEY];
        basicDetails.push_back(std::move(subRec));
    }
    return basicDetails;
 }


nlohmann::json koai::KoAi::autoComplete(std::string athleteName){
    auto fullRecs = dataProvider.failOverSearch(athleteName);
    return getFighterBasicDetails(fullRecs);
}


nlohmann::json koai::KoAi::predict(nlohmann::json&athlete1, nlohmann::json&athlete2){
   auto fullAthlete1Rec = dataProvider.failOverSearch(std::move(athlete1));
   auto fullAthlete2Rec = dataProvider.failOverSearch(std::move(athlete2));
   if(fullAthlete1Rec!=nullptr && fullAthlete2Rec!=nullptr){
      athlete_info::AthleteInfoReader athlete1(std::move(fullAthlete1Rec));
      athlete_info::AthleteInfoReader athlete2(std::move(fullAthlete2Rec));
      return ufcPredicter.predict(athlete1,athlete2);
    }
    return {};
}


nlohmann::json koai::KoAi::getRecentPredictions(){
  return ufcPredicter.getRecentPredictions();
}