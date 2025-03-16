#ifndef __KOAI_UFC_SCRAPER__
#define __KOAI_UFC_SCRAPER__

/**
* @author Mohamed Naamane (mrnmnwork@gmail.com)
* @brief Core Ufc scraping functionality
*/

#include "ufc_search.hpp"
#include "athlete_scraper.hpp"
#include "../utils/record_processor.hpp"
#include "../loggers/logger_modes.hpp"
#include "../loggers/koai_logger.hpp"
#include "../controllers/database_ctl_modes.hpp"
#include "../utils/screen_cleaner.hpp"


namespace ufc_scraper{
    class UfcScraper{
        ufc_search::UfcSearch ufcSearcher;
        athlete_scraper::AthleteScraper athlete;
        std::shared_ptr<logging::KoAiLogger>logger;
        database_ctl::MODE currDbMode;
        std::string defaultKeyword;
        public:
        UfcScraper()=default;
        UfcScraper(std::shared_ptr<logging::KoAiLogger>&logger);
        UfcScraper(const UfcScraper&)=delete;
        UfcScraper&operator=(const UfcScraper&)=delete;
        UfcScraper(UfcScraper&&)noexcept=delete;
        UfcScraper&operator=(UfcScraper&&)noexcept=delete;
        void setTargetAthleteFname(std::string&&athleteName);
        UfcScraper&search(std::string athleteName);
        athlete_scraper::AthleteScraper& setTargetAthleteByIndex(size_t index);
        athlete_scraper::AthleteScraper&getAthlete();
        std::shared_ptr<logging::KoAiLogger>&getLogger();
        void setCurrDbMode(database_ctl::MODE currDbMode);
        database_ctl::MODE getCurrDbMode();
        void forEachRecord(auto recordProcessor);
        //get max records ro read when db mode is live.
        const size_t getMaxLiveRead()const{
            return 5;
        }
        ~UfcScraper()=default;
    };
}




void ufc_scraper::UfcScraper::forEachRecord(auto recordProcessor){
    //make sure each time start with the first result.
    ufcSearcher.returnToFirstPage();
    this->search(defaultKeyword);
    size_t totalRecordsCount = athlete.getAthletsCount();    
    size_t totalProcessed{0};
    record_processer::RecordProcessor recProcessor;
    const size_t  MAX_LIVE_READ = getMaxLiveRead();
    while(totalRecordsCount>0 && totalProcessed<totalRecordsCount){
        //printf("totalProcessed=%zu totalRecordsCount = %zu\n",totalProcessed , totalRecordsCount);
        if(currDbMode ==database_ctl::MODE::LIVE && totalProcessed >= MAX_LIVE_READ){
            break;
        }
        if(currDbMode==database_ctl::MODE::UPDATE){
            screen_cleaner::ScreenCleaner::clearScreen();
            logger->clear(logging::LogMode::FULL);
        }else{
            //add messages to logger
            logger->clear(logging::LogMode::FIRST);
        }

        if(currDbMode != database_ctl::MODE::LIVE){
            double percentage = (totalProcessed*100.)/totalRecordsCount;
            logger->add(totalProcessed,"/",totalRecordsCount," completed: ", percentage,"%\n");
            logger->log(logging::LogMode::FULL);
        }
       
        size_t addedRecords = recProcessor.forEachRecord(*this,recordProcessor);
        totalProcessed += addedRecords;

        if(currDbMode != database_ctl::MODE::LIVE){
         int searhcLimit = athlete.getSearchLImit();
         ufcSearcher.moveToNextPage(searhcLimit);//makes the endPointBuilder generates enpoint fot the athletes range.
         this->search(defaultKeyword);//search again to fetch the next generated enpoint.
        }
    }
   //all records processed.
}


#endif // ! __KOAI_UFC_SCRAPER__ INCLUDED