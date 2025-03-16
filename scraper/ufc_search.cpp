#include "ufc_search.hpp"

ufc_search::UfcSearch::UfcSearch(){
    setUfcAPiKey();
}


std::string ufc_search::UfcSearch::fetch(std::string url){
    auto src = httpClient.get(url).response();
    return std::move(src);
}


std::string ufc_search::UfcSearch::yext_fetch(std::string athleteName){
    endPointBuilder.setAthleteName(athleteName);
    std::string yextEndPoint = endPointBuilder.getYextEndPoint(apiKey);
    //printf("yextEndPoint=%s\n",yextEndPoint.c_str());
    std::string textResponse = httpClient.get(yextEndPoint).response();
    return std::move(textResponse);
  }

std::string ufc_search::UfcSearch::ufc_fetch(std::string athleteName){
    endPointBuilder.setAthleteName(athleteName);
    std::string ufcEndPoint = endPointBuilder.getUfcEndPoint();
    //printf("yextEndPoint=%s\n",yextEndPoint.c_str());
    std::string textResponse = httpClient.get(ufcEndPoint).response();
    return std::move(textResponse);
}

void ufc_search::UfcSearch::moveToNextPage(int& searchLimit){
   //move to the next page. nextPage calculated according to yext api
   std::string currPage = endPointBuilder.getCurrPage();
   size_t currPageNUmber = size_t(std::stoi(currPage));
   size_t nextPage = currPageNUmber+searchLimit;
   std::string nextPageStr = std::to_string(nextPage);
   //printf("nextPageStr= %s\n",nextPageStr.c_str());
   //generate a new ufc end point to fetch the next athlete range
   endPointBuilder.setCurrPage(nextPageStr);
}


void ufc_search::UfcSearch::setUfcAPiKey(){
    this->apiKey=std::string{};
    constexpr auto mainUrl = "https://www.ufc.com";
    std::string resTxt =  httpClient.get(std::string(mainUrl)).response();
    if(!resTxt.empty()){
      utils::RegexSearch rsearch;
      constexpr auto patt(R"(apiKey:\s*"([^"]+)\")");
      std::string apiKy = rsearch.reSearch(resTxt,patt);
      if(!apiKy.empty()){
        //printf("apiKy=%s\n", apiKy.c_str());
        this->apiKey = apiKy;
      }
    }
    this->apiKey= public_keys::UFC_PUBLIC_API_KEY;
}


void ufc_search::UfcSearch::returnToFirstPage(){
  endPointBuilder.setCurrPage("0");
}
