#ifndef __KOAI__RECORD_PROCESSOR__
#define __KOAI__RECORD_PROCESSOR__

#include "../database/database_titles.hpp"
#include "../loggers/logger_modes.hpp"
#include "screen_cleaner.hpp"
#include "../controllers/database_ctl_modes.hpp"
#include "disk/disk_info.hpp"
#include <thread>

namespace record_processer{
    class RecordProcessor{
       bool dataBasePromptShowed;
       public:
       RecordProcessor():dataBasePromptShowed(false){};
       RecordProcessor(const RecordProcessor&)=delete;
       RecordProcessor&operator=(const RecordProcessor&)=delete;
       RecordProcessor(RecordProcessor&&)noexcept=delete;
       RecordProcessor&operator=(RecordProcessor&&)noexcept=delete;
       void showDataBasePrompt(auto&athlete,auto&rec);
       void fillRecord(database_ctl::MODE&mode,auto& athlete,nlohmann::json&record);
       unsigned long forEachRecord(auto&scraperCaller,auto&recordProcessor);
       ~RecordProcessor()=default;
    };
}


void record_processer::RecordProcessor::showDataBasePrompt(auto&athlete,auto&rec){  
    size_t athCounts = athlete.getAthletsCount();
    //calc the estiamted total database size based on the total records size.
    size_t totalEsSIze = sizeof(rec) * athCounts;
    disk_info::DiskInfo diskInfo;
    //read available disk size
    size_t avBytes = diskInfo.getAvailableDiskSizeBytes();  
    size_t avBytesMb = diskInfo.bytesToMegabytes(avBytes);
    printf("Available disk space: %zu MBs\n", avBytesMb);
    printf("Database estimated size: %zu Bytes.\n",totalEsSIze);
    printf("Database creation will begin in 10 seconds. Press Ctrl+C to cancel.\n");
    std::this_thread::sleep_for(std::chrono::seconds{10});
    dataBasePromptShowed=true;
  }

//fill the record with ufc athlete data
void record_processer::RecordProcessor::fillRecord(database_ctl::MODE&mode, auto&athlete,nlohmann::json&record){
    using namespace database_titles;
    if(mode== database_ctl::MODE::LIVE){
        record[FIRST_NAME_KEY]= athlete.getFirstName();
        record[LAST_NAME_KEY]=athlete.getLastName();
        record[GENDER_KEY] = athlete.getGender();
        record[NICKNAME_KEY] = athlete.getNickName();
        auto img = athlete.getImage();
        record[IMAGE_URL_DB_KEY]=img.url;
        return;
    }
    record[FIRST_NAME_KEY]= athlete.getFirstName();
        record[LAST_NAME_KEY]=athlete.getLastName();
        record[DIVISION_KEY] = athlete.getDivision();
        record[NICKNAME_KEY] = athlete.getNickName();
        record[STRIKING_ACCURACY_KEY] = athlete.getStrikingAccuracy();
        record[TAKEDOWN_ACCURACY_KEY] = athlete.getTakedownAccuracy();
        record[SIG_STR_LANDED_PER_MIN_KEY] = athlete.getSigStrLandedPerMin();
        record[SIG_STR_ABSORBED_PER_MIN_KEY] = athlete.getSigStrAbsorbedPerMin();
        record[TAKEDOWN_AVG_PER_15_MIN_KEY] = athlete.getTakedownAvgPer15Min();
        record[SIG_STR_DEFENSE_KEY] = athlete.getSigStrDefense();
        record[KNOCKDOWN_AVG_KEY] = athlete.getKnockdownAvg();
        record[SUBMISSION_AVG_KEY] = athlete.getSubmissionAvgPer15Min();
        record[TAKEDOWN_DEFENSE_KEY] = athlete.getTakeDownDefense();
        record[AVERAGE_FIGHT_TIME_AS_MINUTES_KEY] = athlete.getAverageFightTimeAsMinuts();
        record[SIG_STR_BY_POS_STANDING_KEY] = athlete.getSigStrByPosStanding();
        record[SIG_STR_BY_POS_CLINCH_KEY] = athlete.getSigStrByPosClinch();
        record[SIG_STR_BY_POS_GROUND_KEY] = athlete.getSigStrByPosGround();
        record[SIG_STR_BY_TARGET_HEAD_KEY] = athlete.getSigStrByTargetHead();
        record[SIG_STR_BY_TARGET_BODY_KEY] = athlete.getSigStrByTargetBody();
        record[SIG_STR_BY_TARGET_LEG_KEY] = athlete.getSigStrByTargetLeg();
        record[WIN_BY_KO_TKO_KEY] = athlete.getWinByKoTko();
        record[WIN_BY_DEC_KEY] = athlete.getWinByDec();
        record[WIN_BY_SUB_KEY] = athlete.getWinBySub();
        record[STATUS_KEY] = athlete.getStatus();
        record[FIGHTING_STYLE_KEY] = athlete.getFightingStyle();
        record[AGE_KEY] = athlete.getAge();
        record[WEIGHT_DB_KEY] = athlete.getWeight();
        record[HEIGHT_DB_KEY] = athlete.getHeight();
        record[GENDER_KEY] = athlete.getGender();
        record[WEIGHT_CLASS_DB_KEY] = athlete.getWeightClass();
        auto img = athlete.getImage();
        record[IMAGE_URL_DB_KEY]=img.url;
        record[IMAGE_WIDTH_DB_KEY]=img.width;
        record[IMAGE_HEIGHT_DB_KEY]=img.height;
}

unsigned long record_processer::RecordProcessor::forEachRecord(auto&scraperCaller,auto& recordProcessor){
    if(scraperCaller.getAthlete().isYextPayloadEmpty()){//no athlets.
        printf("Online data provider: ","No athletes.\n");
        return 0;
    }
    size_t currResultsSize = scraperCaller.getAthlete().getCurrResultsSize();
    size_t addedRecords{0};
    const size_t  MAX_LIVE_READ = scraperCaller.getMaxLiveRead();
    database_ctl::MODE currMode = scraperCaller.getCurrDbMode();
    //printf("currResultsSize=%zu\n", currResultsSize);
    for(size_t i=0; i<currResultsSize;++i){
        //check live read limit.
        if(currMode ==database_ctl::MODE::LIVE && i >= MAX_LIVE_READ){
            ++addedRecords;
            break;
        }
        double percentage = ((i+1)*100.)/currResultsSize;
        if(currMode != database_ctl::MODE::UPDATE && 
            currMode != database_ctl::MODE::LIVE){
            screen_cleaner::ScreenCleaner::clearScreen();
            int searchLimit = scraperCaller.getAthlete().getSearchLImit();
            auto logger = scraperCaller.getLogger();
            logger->add(int(i+1) ,"/",searchLimit ," record processing: ",percentage,"%\n");
            logger->log(logging::LogMode::FULL);
            logger->clear(logging::LogMode::RECENT);
        }
        nlohmann::json record{};
        scraperCaller.setTargetAthleteByIndex(i);
        //athlete.setTargetIndex(i); //select the first athlete in yext payload.
        auto&athlete = scraperCaller.getAthlete();
        fillRecord(currMode,athlete, record);
        //show the database size prompt
        if(currMode == database_ctl::MODE::ADD_NEW && !dataBasePromptShowed){
            showDataBasePrompt(athlete, record);
        }
        recordProcessor(std::move(record));
        ++addedRecords;
    }    
   return addedRecords;
}



#endif // ! __KOAI__RECORD_PROCESSOR__ INCLUDED