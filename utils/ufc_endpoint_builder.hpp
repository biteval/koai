#ifndef __KOAI_UFC_ENDPOINT_BUILDER__
#define __KOAI_UFC_ENDPOINT_BUILDER__

/**
* @brief build ufc needed endpoints based on diffrent parameters
*/



#include <string>

namespace utils{
    namespace api_str{
        constexpr auto UFC_SEARCH_HEAD = "https://www.ufc.com/search?query=";
        constexpr auto UFC_SEARCH_TAIL= "&referrerPageUrl=&tabOrder=./index.html,banners,athletes,help_articles,videos,links,events,faqs,promotions&facetFilters={}&filters={}&verticalUrl=Athletes.html";
        constexpr auto YEXT_API_HEAD = "https://liveapi.yext.com/v2/accounts/me/answers/vertical/query?experienceKey=answers-en&api_key=";
        constexpr auto YEXT_API_MIDDLE = "&v=20220511&version=PRODUCTION&locale=en&input=";
        constexpr auto YEXT_API_FRONT_TAIL = "&verticalKey=athletes&limit=21&offset=";
        constexpr auto YEXT_API_BACK_TAIL = "&retrieveFacets=true&facetFilters={}&session_id=&sessionTrackingEnabled=true&sortBys=[]&referrerPageUrl=https://www.ufc.com/&source=STANDARD&jsLibVersion=v1.14.3";
    }
    
    class UfcEndPointBuilder{
     mutable   std::string athleteName;
     mutable   std::string yextCurrPage;
     public:
     UfcEndPointBuilder();
     UfcEndPointBuilder(const UfcEndPointBuilder&)=delete;
     UfcEndPointBuilder& operator=(const UfcEndPointBuilder&)=delete;
     UfcEndPointBuilder(UfcEndPointBuilder&&)=delete;
     UfcEndPointBuilder& operator=(UfcEndPointBuilder&&)=delete;
     void setAthleteName(std::string athleteName)const;
     std::string getUfcEndPoint()const;
     std::string getYextEndPoint(std::string& apiKey)const;
     void setCurrPage(std::string yextCurrPage)const;
     std::string getCurrPage()const;
     ~UfcEndPointBuilder()=default;
   };
}

#endif // ! __KOAI_UFC_ENDPOINT_BUILDER__ INCLUDED
