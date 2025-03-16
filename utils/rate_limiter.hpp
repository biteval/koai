#ifndef __KOAI_RATE_LIMITER__
#define __KOAI_RATE_LIMITER__

/**
* @author Mohamed Naamane (mrnmnwork@gmail.com)
* @brief rate limiter to block multiple fast requests with the same ip address.
*/


#include <string>
#include <unordered_map>
#include <chrono>
#include <mutex>
#include <list>

namespace rate_limiter{
    // LRU Cache for Rate Limiting
    struct RateLimitRecord{
        std::string ip;
        std::chrono::steady_clock::time_point lastRequest;
    };
    class RateLimiter{
        std::unordered_map<std::string, std::list<RateLimitRecord>::iterator>rateLimitMap;
        std::list<RateLimitRecord>rateLimitList;
        const size_t MAX_CACHE_SIZE = 50000;
        std::mutex mtx;
        void removeOldeUser();
        public:
        RateLimiter();
        RateLimiter(const RateLimiter&)=delete;
        RateLimiter& operator=(const RateLimiter&)=delete;
        RateLimiter(RateLimiter&&)noexcept=default;
        RateLimiter& operator=(RateLimiter&&)noexcept=default;
        bool isRateLimited(const std::string& ip, int limitMs);
        ~RateLimiter();
    };
}




#endif // ! __KOAI_RATE_LIMITER__ INCLUDED