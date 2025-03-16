#include "ufc_scraper.hpp"

ufc_scraper::UfcScraper::UfcScraper(std::shared_ptr<logging::KoAiLogger>&logger){
    defaultKeyword="";
    this->logger=logger;
}


athlete_scraper::AthleteScraper& ufc_scraper::UfcScraper::getAthlete(){
  return athlete;
}

std::shared_ptr<logging::KoAiLogger>& ufc_scraper::UfcScraper::getLogger(){
   return logger;
}

void ufc_scraper::UfcScraper::setCurrDbMode(database_ctl::MODE currDbMode){
  this->currDbMode=currDbMode;
}

database_ctl::MODE ufc_scraper::UfcScraper::getCurrDbMode(){
   return currDbMode;
}

void ufc_scraper::UfcScraper::setTargetAthleteFname(std::string&&athleteName){
    auto& nrmlizer = athlete.getStringNormilizer();
    nrmlizer.normalizeText(athleteName);
    nrmlizer.spaceToPlus(athleteName);
    this->defaultKeyword=std::move(athleteName);
}

ufc_scraper::UfcScraper& ufc_scraper::UfcScraper::search(std::string athleteName){
    //printf("searching for %s\n", athleteName.c_str());
    auto yextSrc = ufcSearcher.yext_fetch(athleteName);
    if(yextSrc.empty()){//empty response.
        return *this;
    }
    athlete.parse(std::move(yextSrc));
    return *this;
}

athlete_scraper::AthleteScraper& ufc_scraper::UfcScraper::setTargetAthleteByIndex(size_t index){
    auto landPage = athlete.setTargetIndex(index).getLandPageUrl();
    auto landPageSrc = ufcSearcher.fetch(landPage);
    athlete.setHtml(std::move(landPageSrc));
    return athlete;
}


