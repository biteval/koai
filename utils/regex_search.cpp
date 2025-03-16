#include "regex_search.hpp"

std::string utils::RegexSearch::reSearch(std::string&text,const char *const& pattern)const{
    std::smatch matches{};
    std::regex patt(pattern);
    if(std::regex_search(text,matches,patt)){
        return matches[1].str();
    }
    return std::string{};
}