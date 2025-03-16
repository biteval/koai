#ifndef __KOAI_DATA_PROVIDER_MANAGER__
#define __KOAI_DATA_PROVIDER_MANAGER__

#include "../controllers/database_controller.hpp"
#include "../controllers/controller_configurer.hpp"
#include "../loggers/koai_logger.hpp"
#include "../scraper/ufc_scraper.hpp"
#include "database_manager.hpp"
#include <mutex>

namespace data_provider{
  class DataProviderManager{
    std::shared_ptr<logging::KoAiLogger>sharedLogger;
    ufc_scraper::UfcScraper scraper;
    database_ctl::DataBaseController databaseController;
    database_manager::UfcDatabaseManager ufcDbManager;
    std::mutex offlineMtx, onlineMtx;
    public:
    DataProviderManager():sharedLogger(std::make_shared<logging::KoAiLogger>()) ,scraper(sharedLogger){
      databaseController.setLogger(sharedLogger);
    }
    
    template<typename Configurer>
    DataProviderManager &configureDbController(database_ctl::ControllerConfigurer<Configurer>&configurer){
        configurer.configure(databaseController); //configurer is UfcDataBaseControllerConf object
        return *this;
    }

    void setCredentials(std::string&&hosts, database::Credentials&&cred){
      databaseController.setCredentials(std::move(hosts), std::move(cred));
    }

    std::shared_ptr<logging::KoAiLogger>&getLogger();
    ufc_scraper::UfcScraper&onlineDataProvider();
    //update database for a specific athlete first names , if no athlete first name is provided it will update the db for all ufc athletes.
    void updateDatabase(std::string&&athleteName="");
    //create database for a specific athlete first names , if no athlete first name is provided it will create a db for all ufc athletes.
    void createDatabase(std::string&&athleteName="");
    nlohmann::json offlineSearch(std::string athleteName);
    nlohmann::json offlineSearch(nlohmann::json& athleteRec);

    nlohmann::json onlineSearch(std::string&& athleteName);
    nlohmann::json onlineSearch(nlohmann::json& athleteRec);
    
    nlohmann::json failOverSearch(std::string athleteName);
    nlohmann::json failOverSearch(nlohmann::json&& athleteRec);
    
    //normalize escape the values in the record and return true if it safe.
    bool normalizeAndReport(nlohmann::json&rec);
    DataProviderManager(const DataProviderManager&)=delete;
    DataProviderManager& operator=(const DataProviderManager&)=delete;
    DataProviderManager(DataProviderManager&&)=delete;
    DataProviderManager& operator=(DataProviderManager&&)=delete;
    ~DataProviderManager()=default;
  };
}  

#endif // ! __KOAI_DATA_PROVIDER_MANAGER__ INCLUDED