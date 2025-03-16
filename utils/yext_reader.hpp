#ifndef __KOAI_YEXT_READER__
#define __KOAI_YEXT_READER__

/**
* @author Mohamed Naamane (mrnmnwork@gmail.com)
* @brief json reader to read the yext api used by ufc .
*/


#include "json_search.hpp"
#include "string_manipulator/string_manipulator.hpp"

namespace yext{
    namespace yext_json_keys{
        constexpr auto RESULTS_KEY = "results";
        constexpr auto RESPONSE_KEY = "response";
        constexpr auto DATA_KEY = "data";
        constexpr auto LAND_PAGE_URL_KEY = "landingPageUrl";
        constexpr auto RESULT_COUNT_KEY = "resultsCount";
        constexpr auto NAME_KEY = "name";
        constexpr auto AGE_kEY = "c_age";
        constexpr auto HEIGHT_KEY = "c_height";
        constexpr auto WEIGHT_KEY = "c_weight";
        constexpr auto GENDER_KEY = "gender";
        constexpr auto IMAGE_KEY = "c_photo";
        constexpr auto IMAGE_URL_KEY = "url";
        constexpr auto IMAGE_WIDTH_KEY = "width";
        constexpr auto IMAGE_HEIGHT_KEY = "height";
        constexpr auto WEIGHT_CLASS_KEY = "c_weightClass";
        constexpr auto NICK_NAME_KEY = "c_nickname";
        constexpr auto YEXT_FIRST_NAME = "firstName";
        constexpr auto YEXT_LAST_NAME = "lastName";
    }
    
    class YextReader{
        nlohmann::json payload;
        utils::JsonSearch jsonSearcher;
        int targetAthleteIndex;
        int searchLImit;
        public:
        YextReader();
        YextReader(const YextReader&)=delete;
        YextReader&operator=(const YextReader&)=delete;
        YextReader(YextReader&&)noexcept=delete;
        YextReader&operator=(YextReader&&)noexcept=delete;
        void setTargetAthleteIndex(int targetAthleteIndex);
        void setSearchLImit(int searchLImit);
        int  getTargetAthleteIndex();
        int  getSearchLImit();
        bool isResultsExists();
        unsigned long currResultsSize();
        bool isPayloadEmpty();
        void parse(std::string&&txt);
        
        size_t getAthletsCount();
        std::string getLandPageUrl();
        template<typename T>
        std::string get(std::string finalKey, bool isJson, utils::StringManipulator<T>&strManipulator,bool useNormalizer=true);
        ~YextReader()=default;
    };
}

template<typename T>
std::string yext::YextReader ::get(std::string finalKey, bool isJson, utils::StringManipulator<T>&strManipulator, bool useNormalizer){
    unsigned long athletesCount = getAthletsCount();
    bool resultsKeyExists = payload[yext_json_keys::RESPONSE_KEY].contains(yext_json_keys::RESULTS_KEY); 
    std::string resultVal{};
    if(athletesCount>0 && resultsKeyExists){
        if(jsonSearcher.isChildKeyExists(payload[yext_json_keys::RESPONSE_KEY][yext_json_keys::RESULTS_KEY][targetAthleteIndex],yext_json_keys::DATA_KEY,finalKey)){
           if(isJson){
            resultVal = payload[yext_json_keys::RESPONSE_KEY][yext_json_keys::RESULTS_KEY][targetAthleteIndex][yext_json_keys::DATA_KEY][finalKey].dump();
           }else{
            resultVal =  payload[yext_json_keys::RESPONSE_KEY][yext_json_keys::RESULTS_KEY][targetAthleteIndex][yext_json_keys::DATA_KEY][finalKey];
            if(useNormalizer){
                strManipulator.normalizeText(resultVal);   
            }
        }
        }
    }
    return std::move(resultVal);
}


#endif // ! __KOAI_YEXT_READER__ INCLUDED