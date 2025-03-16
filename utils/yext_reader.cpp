#include "yext_reader.hpp"


yext::YextReader::YextReader():targetAthleteIndex(0), searchLImit(21){}


void yext::YextReader::setSearchLImit(int searchLImit){this->searchLImit=searchLImit;}
int  yext::YextReader::getTargetAthleteIndex(){return targetAthleteIndex;}
int  yext::YextReader::getSearchLImit(){return searchLImit;}

void yext::YextReader::setTargetAthleteIndex(int targetAthleteIndex){
    this->targetAthleteIndex=targetAthleteIndex;
}

void yext::YextReader::parse(std::string&&txt){
    payload=nlohmann::json::parse(std::move(txt));
}


size_t yext::YextReader ::getAthletsCount(){
  size_t resultsCount{0}; 
    if(jsonSearcher.isChildKeyExists(payload,yext_json_keys::RESPONSE_KEY,yext_json_keys::RESULT_COUNT_KEY)){
        resultsCount = payload[yext_json_keys::RESPONSE_KEY][yext_json_keys::RESULT_COUNT_KEY];
     }
     return resultsCount;   
}


std::string yext::YextReader ::getLandPageUrl(){
   //get the landing page url
   std::string landPageUrl{};
   if(jsonSearcher.isChildKeyExists(payload, yext_json_keys::RESPONSE_KEY, yext_json_keys::RESULTS_KEY)){
     //get the athlete landing page url from  yextJsonResponse if the given index is not out of range 
     if(targetAthleteIndex>=0 && targetAthleteIndex < searchLImit && 
        jsonSearcher.isChildKeyExists(payload[yext_json_keys::RESPONSE_KEY][yext_json_keys::RESULTS_KEY][targetAthleteIndex],yext_json_keys::DATA_KEY,yext_json_keys::LAND_PAGE_URL_KEY)){ 
        landPageUrl = payload[yext_json_keys::RESPONSE_KEY][yext_json_keys::RESULTS_KEY][targetAthleteIndex][yext_json_keys::DATA_KEY][yext_json_keys::LAND_PAGE_URL_KEY];
      }
   }
   return std::move(landPageUrl);
}


bool yext::YextReader ::isResultsExists(){
    return payload[yext_json_keys::RESPONSE_KEY].contains(yext_json_keys::RESULTS_KEY);
}


unsigned long yext::YextReader ::currResultsSize(){
  if(this->isResultsExists()){
    return payload[yext_json_keys::RESPONSE_KEY][yext_json_keys::RESULTS_KEY].size(); 
  }
  return 0;
}


bool yext::YextReader ::isPayloadEmpty(){
   return payload.empty();
}
