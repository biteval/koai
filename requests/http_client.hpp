#ifndef __KOAI__HTTP_CLIENT__
#define __KOAI__HTTP_CLIENT__

/*
a simple configured http server just to get the html 
code of a target page.
*/

#include <curl/curl.h>
#include <string>
#include <nlohmann/json.hpp>

namespace requests{
  namespace rsc{
    constexpr auto cacertFile = "requests/assets/cacert.pem";
    constexpr unsigned short HTMLOK = 200;
  }

  namespace json_keys{
    constexpr auto JKEY_BODY = "body";
    constexpr auto JKEY_STATUS_CODE = "scode";
  } 
  class HttpClient{
    CURL* curl_;
    //json obj to store the source code and status code etc
    nlohmann::json jsResponse;
    static size_t writeCallBack(void* contents, size_t size, size_t nmemb, void* userp);
    public:
    HttpClient();
    HttpClient(const HttpClient&)=delete;
    HttpClient&operator=(const HttpClient&)=delete;
    HttpClient(HttpClient&&)=delete;
    HttpClient&operator=(HttpClient&&)=delete;
    //perform get request to a given url
    requests::HttpClient& get(std::string url);
    //return the source code by moving it if status code is 200
    std::string response();
    ~HttpClient(); // destroy curl
  };
}


#endif // ! __KOAI__HTTP_CLIENT__ INCLUDED