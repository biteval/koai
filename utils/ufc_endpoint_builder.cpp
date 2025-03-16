#include "ufc_endpoint_builder.hpp"

utils::UfcEndPointBuilder::UfcEndPointBuilder(){
    athleteName=std::string{};
    yextCurrPage = "0";
}


void utils::UfcEndPointBuilder::setCurrPage(std::string yextCurrPage)const{
    this->yextCurrPage=yextCurrPage;
}


std::string utils::UfcEndPointBuilder::getCurrPage()const{
  return yextCurrPage;
}


void utils::UfcEndPointBuilder::setAthleteName(std::string athleteName)const{
    this->athleteName=athleteName;
}



std::string utils::UfcEndPointBuilder::getUfcEndPoint()const{
    std::string endPoint = std::string(api_str::UFC_SEARCH_HEAD) + athleteName + std::string(api_str::UFC_SEARCH_TAIL);
    return endPoint;
}

std::string utils::UfcEndPointBuilder::getYextEndPoint(std::string& apiKey)const{
    std::string endPoint = std::string(api_str::YEXT_API_HEAD) + apiKey + std::string(api_str::YEXT_API_MIDDLE) + athleteName + std::string(api_str::YEXT_API_FRONT_TAIL)+yextCurrPage+std::string(api_str::YEXT_API_BACK_TAIL);
    //printf("getYextEndPoint = %s\n", endPoint.c_str());
    return endPoint;
}