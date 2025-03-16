#ifndef __KOAI_UFC_SEARCH__
#define __KOAI_UFC_SEARCH__


#include "../requests/http_client.hpp"
#include "../utils/ufc_endpoint_builder.hpp"
#include "../utils/regex_search.hpp"

namespace ufc_search{
    namespace public_keys{
          //default public ufc api key.
          constexpr auto UFC_PUBLIC_API_KEY = "850a88aeb3c29599ce2db46832aa229f";
    }
    class UfcSearch{
      requests::HttpClient httpClient;
      utils::UfcEndPointBuilder endPointBuilder;
      std::string apiKey;
      void setUfcAPiKey();
      public:
      UfcSearch();
      UfcSearch(const UfcSearch&)=delete;
      UfcSearch&operator=(const UfcSearch&)=delete;
      UfcSearch(UfcSearch&&)noexcept=delete;
      UfcSearch&operator=(UfcSearch&&)noexcept=delete;
      //return the src code of the fetched url.
      std::string fetch(std::string url);
      //return the get response of a ufc athlete page
      std::string ufc_fetch(std::string athleteName); 
      //return the get response of a yext endpoint.
      std::string yext_fetch(std::string athleteName);
      //move to the next page. nextPage calculated according to yext api.
      void moveToNextPage(int& searchLimit);
      void returnToFirstPage();
      ~UfcSearch(){}
    };
}

#endif // ! __KOAI_UFC_SEARCH__ INCLUDED