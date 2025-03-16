#include "json_search.hpp"

bool utils::JsonSearch::isChildKeyExists(nlohmann::json&js,std::string parentKey, std::string childKey){
    if(js.contains(parentKey)){
        return js[parentKey].contains(childKey);
    }
    return false;
}


std::string utils::JsonSearch::doReadStr(nlohmann::json&record,const char* key)const{
    if (record != nullptr && record.contains(key) && record[key].is_string()) {
        return record[key].get<std::string>();
    }
    return {};
}

std::string utils::JsonSearch::readStr(nlohmann::json&record,const char* key)const{
    try{
      auto res = doReadStr(record,key);
      return res;
    }catch(...){}
    return {}; 
}

