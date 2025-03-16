#include "data_provider_manager.hpp"

std::shared_ptr<logging::KoAiLogger>& data_provider::DataProviderManager::getLogger(){
    return sharedLogger;
}
  
ufc_scraper::UfcScraper& data_provider::DataProviderManager::onlineDataProvider(){
    return scraper;
}


bool data_provider::DataProviderManager::normalizeAndReport(nlohmann::json&rec){
    const size_t EXPECTED_SIZE = 5;
    if(rec.size()!= EXPECTED_SIZE){ //expected 5 pairs.
        return false;
    }
    auto& strNormalizer = scraper.getAthlete().getStringNormilizer();
    if(!strNormalizer.isGoodValueSize(rec[database_titles::FIRST_NAME_KEY].get_ref<std::string&>()) || 
    !strNormalizer.isGoodValueSize(rec[database_titles::LAST_NAME_KEY].get_ref<std::string&>()) || 
    !strNormalizer.isGoodValueSize(rec[database_titles::GENDER_KEY].get_ref<std::string&>()) || 
    !strNormalizer.isGoodUrlSize(rec[database_titles::IMAGE_URL_DB_KEY].get_ref<std::string&>()) || 
    !strNormalizer.isGoodValueSize(rec[database_titles::NICKNAME_KEY].get_ref<std::string&>())){
        return false;
    }
    strNormalizer.normalizeText(rec[database_titles::FIRST_NAME_KEY].get_ref<std::string&>());
    strNormalizer.normalizeText(rec[database_titles::LAST_NAME_KEY].get_ref<std::string&>());
    strNormalizer.normalizeText(rec[database_titles::GENDER_KEY].get_ref<std::string&>());
    strNormalizer.removeSpaces(rec[database_titles::FIRST_NAME_KEY].get_ref<std::string&>());
    strNormalizer.removeSpaces(rec[database_titles::GENDER_KEY].get_ref<std::string&>());
    return true;
    }

nlohmann::json data_provider::DataProviderManager::onlineSearch(std::string&& athleteName){
    scraper.setTargetAthleteFname(std::move(athleteName));
    auto onlineResults = nlohmann::json::array();
    scraper.forEachRecord([&onlineResults](auto&&rec){
        onlineResults.push_back(std::move(rec));
    });
    return onlineResults;
    }

nlohmann::json data_provider::DataProviderManager::failOverSearch(std::string athleteName){
bool isGoodSize = scraper.getAthlete().getStringNormilizer().isGoodValueSize(athleteName); 
if(!isGoodSize){
    printf("athlete name invalid size\n");
    return {};
}

/*auto&athlete = scraper.getAthlete();
athlete.getStringNormilizer().spaceToPlus(athleteName);*/
//set the curr mode live to limit the live record reading because failOverSearch used in production.
std::unique_lock<std::mutex>offlineLock(offlineMtx);
auto offlineRes = offlineSearch(athleteName);
offlineLock.unlock();
    if(offlineRes!=nullptr && offlineRes.empty()){
    std::unique_lock<std::mutex>onlineLock(onlineMtx);
    scraper.setCurrDbMode(database_ctl::MODE::LIVE);
    return onlineSearch(std::move(athleteName));
    }
    return offlineRes;
}


nlohmann::json data_provider::DataProviderManager::onlineSearch(nlohmann::json& athleteRec){
    utils::JsonSearch jsonSearch;
    std::string fname = jsonSearch.readStr(athleteRec,database_titles::FIRST_NAME_KEY);
    std::string nickName = jsonSearch.readStr(athleteRec,database_titles::NICKNAME_KEY);
    std::string lname = jsonSearch.readStr(athleteRec,database_titles::LAST_NAME_KEY);
    std::string nickNameStr = nickName.empty() ? " " : " " +nickName+" ";
    std::string fullName =  fname+" " + nickNameStr + lname;
    auto res =   onlineSearch(std::move(fullName));
    return res[0];
    }


    nlohmann::json data_provider::DataProviderManager::offlineSearch(std::string athleteName){
        auto&athlete = scraper.getAthlete();
        athlete.getStringNormilizer().normalizeText(athleteName);
        athlete.getStringNormilizer().removeSpaces(athleteName);
        return ufcDbManager.offlineSearch(databaseController,athleteName);
      } 


      nlohmann::json data_provider::DataProviderManager::offlineSearch(nlohmann::json& athleteRec){
        return ufcDbManager.offlineSearch(databaseController,athleteRec);
      }
      
      
      nlohmann::json data_provider::DataProviderManager::failOverSearch(nlohmann::json&& athleteRec){
        nlohmann::json currRec = std::move(athleteRec);
        //normalizatoin and escaping returns true if it safe.
        
        if(!normalizeAndReport(currRec)){
            sharedLogger->add("unsafe record from user, size =", currRec.size());
            sharedLogger->log();
            sharedLogger->clear();
            return {};
        }
        //remove unwanted keys.
        currRec.erase(database_titles::IMAGE_URL_DB_KEY);
        //keep the nickname cached.
        utils::JsonSearch jsonSearch;
        std::string nickName = jsonSearch.readStr(currRec,database_titles::NICKNAME_KEY);
        currRec.erase(database_titles::NICKNAME_KEY);
        std::unique_lock<std::mutex>offlineLock(offlineMtx);
        auto offlineRes = offlineSearch(currRec);
        offlineLock.unlock();
            if(offlineRes !=nullptr && offlineRes.empty()){
            //restore the nick name for online search.
            currRec[database_titles::NICKNAME_KEY]=nickName;
            std::unique_lock<std::mutex>onlineLock(onlineMtx);
            scraper.setCurrDbMode(database_ctl::MODE::LIVE);
            return onlineSearch(currRec);
            }
            return offlineRes[0];
        }
      
void data_provider::DataProviderManager::updateDatabase(std::string&&athleteName){
    scraper.setTargetAthleteFname(std::move(athleteName));
    ufcDbManager.updateDatabase(databaseController, scraper);
}
    
void data_provider::DataProviderManager::createDatabase(std::string&&athleteName){
    scraper.setTargetAthleteFname(std::move(athleteName));
    ufcDbManager.createDatabase(databaseController, scraper);
}