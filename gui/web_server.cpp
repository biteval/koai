#include "web_server.hpp"

webserver::WebServer::WebServer(){
    /*
    // Get reference to CORS middleware
    auto& cors = app.get_middleware<crow::CORSHandler>();
    
    // Configure CORS globally with default permissive settings
    cors.global()
        .headers("Content-Type", "Authorization")
        .methods("GET"_method, "POST"_method, "OPTIONS"_method);
    
    // Add stricter CORS restrictions for protected routes
    cors.prefix("/athlete_search")
        .origin("https://yourdomain.com")  // Restrict to your domain
        .max_age(3600);                    // Cache preflight requests for 1 hour
    
    cors.prefix("/predict")
        .origin("https://yourdomain.com")
        .max_age(3600);
    
    cors.prefix("/recent_pred")
        .origin("https://yourdomain.com")
        .max_age(3600);
    */
    koaiApp = std::make_unique<koai::KoAi>();
}

auto webserver::WebServer::renderPage(std::string page){
    auto loadedPage = crow::mustache::load(page);
    return loadedPage.render();
}

auto webserver::WebServer::athleteSearchRoute(const crow::request &req){
    std::string clientIP = req.remote_ip_address;
    // printf("ip=%s\n", clientIP.c_str());
    if (rateLimiter.isRateLimited(clientIP, 300))
    {
        return crow::response(429, "Too many requests.");
    }

    auto jsonBody = crow::json::load(req.body);

    if (!jsonBody){
        return crow::response{400, "bad request."};
    }

    try{
        // read the input search name
        std::string athleteName = jsonBody["q"].s();
        nlohmann::json sugg = koaiApp->autoComplete(athleteName);
        crow::response res{200};
        res.set_header("Content-Type", "application/json"); // Force JSON content type
        res.body = sugg.dump();
        return res;
    }
    catch (std::exception &ex){
        printf("err:%s\n", ex.what());
    }
    return crow::response{500, "ise"};
}

auto webserver::WebServer::recentPreditionsRoute(){
    const std::string RP_key = "recpred";
    if (rateLimiter.isRateLimited(RP_key, 30000)){
        return crow::response(429, "Too many requests.");
    }
    std::string recentPreds = koaiApp->getRecentPredictions().dump();
    crow::response res{200};
    res.set_header("Content-Type", "application/json");
    res.body = recentPreds;
    return res;
}

auto webserver::WebServer::predictRoute(const crow::request &req){
    std::string clientIP = req.remote_ip_address;
    const std::string CLIENT_KEY = clientIP + ".pre";
    crow::response res{200}; // Set HTTP 200 OK
    res.set_header("Content-Type", "application/json");
    if (rateLimiter.isRateLimited(CLIENT_KEY, 10000)){
        res.body = "{\"text\":\"Sorry, please wait at least 1 second before trying again.\"}";
        return res;
    }
    auto jsonBody = crow::json::load(req.body);
    if (!jsonBody){
        return crow::response{400, "bad request payload."};
    }
    try{
        crow::json::wvalue wJsonBody = std::move(jsonBody);
        nlohmann::json query = nlohmann::json::parse(wJsonBody.dump());
        nlohmann::json rec1 = query["q"][0];
        nlohmann::json rec2 = query["q"][1];
        nlohmann::json predictRes = koaiApp->predict(rec1, rec2);
        if(predictRes!=nullptr){
            res.body = predictRes.dump();
            return res;
        }
    }
    catch (std::exception &ex){
        printf("err:%s\n", ex.what());
    }
    res.body = "{\"text\":\"Sorry, an error occurred during prediction.\"}";
    return res;
}

void webserver::WebServer::listen(){

    CROW_ROUTE(app, "/")
    ([this]()
     { return renderPage("index.html"); });

    // Serve login.html
    CROW_ROUTE(app, "/privacy-policy.html")
    ([this]()
     { return renderPage("privacy-policy.html"); });

    // Serve terms.html
    CROW_ROUTE(app, "/terms.html")
    ([this]()
     { return renderPage("terms.html"); });
   
  
    // Serve /contact.html
    CROW_ROUTE(app, "/contact.html")
    ([this]()
     { return renderPage("contact.html"); });

      // Serve /about.html
    CROW_ROUTE(app, "/about.html")
    ([this]()
    { return renderPage("about.html"); });

   
     
    CROW_ROUTE(app, "/athlete_search")
        .methods(crow::HTTPMethod::Post)([this](const crow::request &req)
                                         { return athleteSearchRoute(req); });

    CROW_ROUTE(app, "/predict")
        .methods(crow::HTTPMethod::Post)([this](const crow::request &req)
                                         { return predictRoute(req); });

    CROW_ROUTE(app, "/recent_pred").methods(crow::HTTPMethod::GET)([this]()
                                                                   { return recentPreditionsRoute(); });

    // enables all log
    app.loglevel(crow::LogLevel::Debug);
    // crow::logger::setHandler(std::make_shared<ExampleLogHandler>());
    //  Use .pem file
    constexpr auto assetsFolderName ="assets";
    auto certPath = std::filesystem::current_path() / assetsFolderName / "c.pem";
    auto keyPath = std::filesystem::current_path() / assetsFolderName / "key.pem";
    const char* port_env = std::getenv("PORT");
    uint16_t lsnPort = port_env ? (uint16_t)std::stoi(port_env) : 18080;
    app.port(lsnPort).ssl_file(certPath.string().c_str(),
                               keyPath.string().c_str())
        .bindaddr("0.0.0.0")
        .multithreaded()
        .run();
}
