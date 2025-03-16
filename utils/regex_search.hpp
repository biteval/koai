#ifndef __KOAI__REGEX_SEARCH__
#define __KOAI__REGEX_SEARCH__

#include <regex>
#include <string>

namespace utils{    
    class RegexSearch{
        public:
        RegexSearch()=default;
        std::string reSearch(std::string&text,const char *const& pattern)const;
        RegexSearch(const RegexSearch&)=delete;
        RegexSearch operator=(const RegexSearch&)=delete;
        RegexSearch(RegexSearch&&)=delete;
        RegexSearch operator=(RegexSearch&&)=delete;
        ~RegexSearch()=default;
    };
}


#endif // ! __KOAI__REGEX_SEARCH__ INCLUDED