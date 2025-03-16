#ifndef __KOAI_STRING_NORMALIZER__
#define __KOAI_STRING_NORMALIZER__

#include "string_manipulator.hpp"
#include <string>

namespace utils{
    class StringNormalizer : public StringManipulator<StringNormalizer>{
        public:
        StringNormalizer()=default;
        StringNormalizer(const StringNormalizer&)=delete;
        StringNormalizer& operator=(const StringNormalizer&)=delete;
        StringNormalizer(StringNormalizer&&)noexcept=default;
        StringNormalizer& operator=(StringNormalizer&&)noexcept=default;
        void  removePerCentSign(std::string&txt);
        void  normalizeText(std::string& input);
        //helper funciton to remove spaces from number string
        void  removeSpacesFromNumberStr(std::string&val);
        //helper funciton to remove spaces or new lines from string
        void  removeSpaces(std::string&val);
        //covert spaces to + for url compatible params 
        void spaceToPlus(std::string&val);
        //convert time like 3:15 to double 3.15
        void timeToStr(std::string&val);

        double strToDouble(std::string&val);
        //check if a value size exceeds the max size
        bool isGoodValueSize(std::string&val);
        bool isGoodUrlSize(std::string&val);

        ~StringNormalizer()=default;
    };
}



#endif // ! __KOAI_STRING_NORMALIZER__ INCLUDED