#ifndef __KOAI_DATABASE_CREATER__
#define __KOAI_DATABASE_CREATER__

#include "../controllers/database_ctl_modes.hpp"
#include <utility>

namespace database_manager{
   class UfcDatabaseManager{
    public:
    UfcDatabaseManager()=default;
    UfcDatabaseManager(const UfcDatabaseManager&)=delete;
    UfcDatabaseManager& operator=(const UfcDatabaseManager&)=delete;
    UfcDatabaseManager(UfcDatabaseManager&&)noexcept=delete;
    UfcDatabaseManager& operator=(UfcDatabaseManager&&)noexcept=delete;
    void createDatabase(auto&dbCtl, auto&scraper);
    void updateDatabase(auto&dbCtl, auto&scraper);
    auto offlineSearch(auto&dbCtl, auto athleteName);
    //ask user to create the database on its own risk based on the available disk space.
    void useSelectChoices(size_t& athletesCount, auto&rec);
    ~UfcDatabaseManager()=default;
   };
}


void database_manager::
UfcDatabaseManager::createDatabase(auto&dbCtl, auto&scraper){
  dbCtl.changeClusterName();
  dbCtl.createKeySpace();
  dbCtl.createTable();
  dbCtl.setMode(database_ctl::MODE::ADD_NEW);
  scraper.setCurrDbMode(dbCtl.getCurrMode());  
  scraper.forEachRecord([&dbCtl](auto&&rec){  
    dbCtl.addRecord(std::move(rec));
  });
}


void database_manager::
UfcDatabaseManager::updateDatabase(auto&dbCtl, auto&scraper){
  dbCtl.setMode(database_ctl::MODE::UPDATE);
  scraper.setCurrDbMode(dbCtl.getCurrMode());
  scraper.forEachRecord([&dbCtl](auto&&rec){
    dbCtl.updateRecord(std::move(rec));
  });
}

auto database_manager::UfcDatabaseManager::offlineSearch(auto&dbCtl, auto athleteName){
  database_ctl::MODE currMode = dbCtl.getCurrMode();
  dbCtl.setMode(database_ctl::MODE::RETRIEVE); 
  auto offRes = dbCtl.search(athleteName);
  dbCtl.setMode(currMode); 
  return offRes;
}

#endif // ! __KOAI_DATABASE_CREATER__ INCLUDED