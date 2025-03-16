#include "string_normalizer.hpp"


void utils::StringNormalizer::removePerCentSign(std::string&txt){
    char perCentSign='%';
    size_t perCentSignPos = txt.find(perCentSign);
    if(perCentSignPos!=std::string::npos){
        txt=txt.substr(0,perCentSignPos);
    }
}


void utils::StringNormalizer::normalizeText(std::string& input){
    std::erase_if(input, [](char& c) {
        c=std::tolower(c);
        return !(std::isalnum(c) || c == ' ');
    });
}


void utils::StringNormalizer::removeSpacesFromNumberStr(std::string&val){
size_t i=0;
    for(;i<val.size();++i){
    if(!std::isdigit(val[i])){
      val=val.substr(0,i);
      break;
    }
}
}


void utils::StringNormalizer::removeSpaces(std::string&val){
    size_t i=0;
    for(;i<val.size();++i){
    if(val[i]==' ' || val[i]=='\n'){
     val=val.substr(0,i);
     break;
    }
    }
}

void utils::StringNormalizer::spaceToPlus(std::string&val){
   for(int i=0; i<val.size();++i){
     int nextCharPos = i+1;
     //escape for double spaces.
     if(nextCharPos<val.size() && val[i]==' ' && val[nextCharPos]==' '){
        continue;
     }else{
        if(val[i]==' '){
            if(i>0 && val[i-1]!=' ' && val[i]==' '){
                val[i]='+';  
            }
            
        }
     }
   }
}

void utils::StringNormalizer::timeToStr(std::string&val){
   for(auto&c:val){
    if(c==':'){
        c='.';
        break;
    }
   }
}

double utils::StringNormalizer::strToDouble(std::string&val){
     return std::stod(val);
}

bool utils::StringNormalizer::isGoodValueSize(std::string&val){
  constexpr size_t MAX_SIZE = 64;
  return val.size()<MAX_SIZE;
} 

bool utils::StringNormalizer::isGoodUrlSize(std::string&val){
    constexpr size_t MAX_SIZE = 1024;
    return val.size()<MAX_SIZE;
}
