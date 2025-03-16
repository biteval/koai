#include "rate_limiter.hpp"

rate_limiter::RateLimiter::RateLimiter():rateLimitList(std::list<RateLimitRecord>{}), rateLimitMap(std::unordered_map<std::string, std::list<RateLimitRecord>::iterator>{}){
    rateLimitMap.reserve(MAX_CACHE_SIZE);
}

void rate_limiter::RateLimiter::removeOldeUser() {
    if (rateLimitList.empty() || rateLimitMap.empty()) return;

    if (rateLimitList.size() > MAX_CACHE_SIZE) {  
        auto oldest = std::prev(rateLimitList.end()); 
        auto it = rateLimitMap.find(oldest->ip);
        if (it != rateLimitMap.end()) {  
            rateLimitMap.erase(it);  
        }
        rateLimitList.pop_back(); 
    }
}


bool rate_limiter::RateLimiter::isRateLimited(const std::string& ip, int limitMs) {
    std::lock_guard<std::mutex> lock(mtx);
    auto now = std::chrono::steady_clock::now();
    if (rateLimitMap.find(ip) != rateLimitMap.end()) {
        auto it = rateLimitMap[ip];
        auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - it->lastRequest).count();
        if (elapsedMs < limitMs){
            return true;
        } 
        rateLimitList.erase(it); // Remove old record
    }
    // Insert new request at front
    rateLimitList.push_front({ip, now});
    rateLimitMap[ip] = rateLimitList.begin();
    // remove oldest entry if cache size >= MAX_CACHE_SIZE limit
    removeOldeUser();
    return false;
}

rate_limiter::RateLimiter::~RateLimiter()=default;