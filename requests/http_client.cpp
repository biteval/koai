#include "http_client.hpp"


requests::HttpClient::HttpClient(){
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl_ = curl_easy_init();
    jsResponse=nlohmann::json{};
    jsResponse[requests::json_keys::JKEY_STATUS_CODE]=std::string{};
    jsResponse[requests::json_keys::JKEY_STATUS_CODE]=long{};
}

/*size_t requests::HttpClient::writeCallBack(void* contents, size_t size, size_t nmemb, void* userp) {
    if (!userp || !contents) {
      return 0;
   }
  size_t totalSize = size * nmemb;
  auto* resData = static_cast<std::string*>(userp);
  try {
      resData->append(static_cast<char*>(contents), totalSize);
  } catch (const std::bad_alloc&) {
      return 0; 
  }
  return totalSize;
}*/


size_t requests::HttpClient::writeCallBack(void* contents, size_t size, size_t nmemb, void* userp) {
  size_t totalSize = size * nmemb;
  std::string* response = static_cast<std::string*>(userp);
  constexpr size_t KB = 1024 ;
  constexpr size_t MAX_RESPONSE_SIZE = 10 * KB * KB;  // 10MB limit

  if (response->size() + totalSize > MAX_RESPONSE_SIZE) {
      printf("Warning: Response too large, stopping download.");
      return 0;  // Stop further writing
  }

  try {
      response->append(static_cast<char*>(contents), totalSize);
  } catch (const std::bad_alloc&) {
      printf("Error: Memory allocation failed while appending response.");
      return 0;
  }

  return totalSize;
}


requests::HttpClient&  requests::HttpClient::get(std::string url){
  if (!curl_) {
    return *this;
  }
  jsResponse[requests::json_keys::JKEY_STATUS_CODE]=std::string{};
  jsResponse[requests::json_keys::JKEY_STATUS_CODE]=long{};
  std::filesystem::path  caCertPath = std::filesystem::current_path() /  requests::rsc::cacertFile;
  curl_easy_setopt(curl_, CURLOPT_CAINFO, caCertPath.string().c_str());
  curl_easy_setopt(curl_, CURLOPT_ACCEPT_ENCODING, "");
  curl_easy_setopt(curl_, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
  std::string  htmlResponse{};
  curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, writeCallBack);
  curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &htmlResponse);
  CURLcode res = curl_easy_perform(curl_);
  if (res != CURLE_OK) {
      return *this;
  }
  jsResponse[requests::json_keys::JKEY_BODY]=std::move(htmlResponse);   
  long statusCode{};
  curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &statusCode);  
  jsResponse[requests::json_keys::JKEY_STATUS_CODE]=statusCode;
  return *this;
}

std::string requests::HttpClient::response(){
  if((unsigned short)jsResponse[requests::json_keys::JKEY_STATUS_CODE]==requests::rsc::HTMLOK){
    return std::move(jsResponse[requests::json_keys::JKEY_BODY].get_ref<std::string&>());
  }
  return std::string{};
}

requests::HttpClient::~HttpClient(){
    if (curl_) {
        curl_easy_cleanup(curl_);
    }
    curl_global_cleanup();
}
