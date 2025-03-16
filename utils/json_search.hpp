#ifndef  __KOAI_JSON_SEARCH__
#define  __KOAI_JSON_SEARCH__

#include <string>
#include <nlohmann/json.hpp>

namespace utils{
   class JsonSearch{
    std::string doReadStr(nlohmann::json&record,const char* key)const;
    public:
    JsonSearch()=default;
    //check if js[parentKey].contains(childKey) without error if js dont contains parentKey 
    bool isChildKeyExists(nlohmann::json&js,std::string parentKey, std::string childKey);
    std::string readStr(nlohmann::json&record,const char* key)const;
    JsonSearch(const JsonSearch&)=delete;
    JsonSearch& operator=(const JsonSearch&)=delete;
    JsonSearch(JsonSearch&&)=delete;
    JsonSearch& operator=(JsonSearch&&)=delete;
    ~JsonSearch()=default;
   };
}



#endif // ! __KOAI_JSON_SEARCH__ INCLDUED