#ifndef __KOAI_ATHLETE_SCRAPER__
#define __KOAI_ATHLETE_SCRAPER__

#include "html_scraper.hpp"
#include "../utils/yext_reader.hpp"
#include "../utils/string_manipulator/string_normalizer.hpp"

namespace athlete_scraper{
    class AthleteScraper{
        html::HtmlScraper htmlScraper;
        yext::YextReader yextReader;
        utils::StringNormalizer strNormilizer;
        //helper fucntion to get a string which is a part of html class name
        std::string getCompareCards1(std::string x);
        //helper fucntion to get a string which is a part of html class name
        std::string getCompareCards2(std::string x);
        public:
        AthleteScraper()=default;
        AthleteScraper(const AthleteScraper &) = delete;
        AthleteScraper &operator=(const AthleteScraper &) = delete;
        AthleteScraper(AthleteScraper &&) noexcept = delete;
        AthleteScraper &operator=(AthleteScraper &&) noexcept = delete;
        //parse the yext api json response.
        void parse(std::string&&yextSrc);

        //athletes count in yext payload
        size_t getAthletsCount();
        //check if the yext payload contains the key RESULTS_KEY.
        bool isResultsExists();

        //check if the yext payload is empty.
        bool isYextPayloadEmpty();

        size_t getCurrResultsSize();

        int getSearchLImit();
        
        utils::StringNormalizer&getStringNormilizer();
        std::string getLandPageUrl();
        //set target athlete index yextReader payload
        AthleteScraper& setTargetIndex(size_t& index);
        void setHtml(std::string&&src);
        // std::string getFullName();
        std::string getFirstName();

        std::string getLastName();

        std::string getDivision();

        std::string getNickName();

        // percentage
        std::string getStrikingAccuracy();

        // percentage
        std::string getTakedownAccuracy();

        std::string getSigStrLandedPerMin();

        std::string getSigStrAbsorbedPerMin();

        std::string getTakedownAvgPer15Min();

        // percentage
        std::string getSigStrDefense();

        std::string getKnockdownAvg();

        std::string getSubmissionAvgPer15Min();

        // percentage
        std::string getTakeDownDefense();

        std::string getAverageFightTimeAsMinuts();

        std::string getSigStrByPosStanding();

        std::string getSigStrByPosClinch();

        std::string getSigStrByPosGround();

        std::string getSigStrByTargetHead();

        std::string getSigStrByTargetBody();

        std::string getSigStrByTargetLeg();

        std::string getWinByKoTko();

        std::string getWinByDec();

        std::string getWinBySub();

        std::string getStatus();

        std::string getFightingStyle();

        std::string getAge();

        std::string getHeight();

        std::string getWeight();

        std::string getGender();

        std::string getWeightClass();

        html::Image getImage();

        ~AthleteScraper()=default;
    };
}

#endif // ! __KOAI_ATHLETE_SCRAPER__ INCLUDED