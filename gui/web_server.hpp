#ifndef __KOAI_CROW_SERVER__
#define __KOAI_CROW_SERVER__


#ifndef CROW_ENABLE_SSL
#define CROW_ENABLE_SSL
#endif // end of CROW_ENABLE_SSL


#include <crow.h>
#include <nlohmann/json.hpp>
#include <memory>
#include "../utils/rate_limiter.hpp"
#include "../database/database_titles.hpp"
#include "../app/koai.hpp"

namespace webserver{
    class WebServer{
        crow::SimpleApp app;
        std::unique_ptr<koai::KoAi>koaiApp;
        rate_limiter::RateLimiter rateLimiter;
        std::mutex mtx;
        auto renderPage(std::string page);
        auto athleteSearchRoute(const crow::request& req);
        auto predictRoute(const crow::request& req);
        auto recentPreditionsRoute();
        public:
        WebServer();
        WebServer(const WebServer&)=delete;
        WebServer operator=(const WebServer&) =delete;
        WebServer(WebServer&&)=delete;
        WebServer& operator=(WebServer&&) =delete;
        void listen(uint16_t lsnPort);
        ~WebServer()=default;
    };
}

#endif // ! __KOAI_CROW_SERVER__ INCLUDED