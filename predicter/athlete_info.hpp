#ifndef __KOAI_ATHLETE_INFO__
#define __KOAI_ATHLETE_INFO__

#include "../database/database_titles.hpp"
#include <string>
#include "../utils/json_search.hpp"

namespace athlete_info{
    class AthleteInfoReader{
        mutable nlohmann::json record; //mutable record to avoid not existing key assertion in a const memeber function.
        double strToDouble(std::string& val)const;
        int strToInt(std::string& val)const;
        utils::JsonSearch jsonSearch; 
        public:
        AthleteInfoReader(nlohmann::json&&record);
        
        size_t recordSize()const;

        std::string getFirstName()const;

        std::string getLastName()const;

        std::string getDivision()const;

        std::string getNickName()const;

        std::string getFullNameWithNickName()const;

        std::string getFullName()const;

        int getStrikingAccurancy()const;
        
        int getTakedownAccuracy()const;

        double getSigStrLandedPerMin()const;

        double getSigStrAbsorbedPerMin()const;

        double getTakedownAvgPer15Min()const;
        // percentage
        int getSigStrDefense()const;

        double getKnockdownAvg()const;

        double getSubmissionAvgPer15Min()const;

        // percentage
        int getTakeDownDefense()const;
        
        double getAverageFightTimeAsMinuts()const;

        int getSigStrByPosStanding()const;

        int getSigStrByPosClinch()const;

        int getSigStrByPosGround()const;

        int getSigStrByTargetHead()const;

        int getSigStrByTargetBody()const;

        int getSigStrByTargetLeg()const;

        int getWinByKoTko()const;

        int getWinByDec()const;

        int getWinBySub()const;

        std::string getStatus()const;

        std::string getFightingStyle()const;

        unsigned short getAge()const;

        double getHeight()const;

        double getWeight()const;

        std::string getGender()const;

        std::string getWeightClass()const;

        std::string getImageUrl()const;
        
        /*int getStrAccurancyAdvantage(const AthleteInfoReader&opponent)const;
        double getStrAccurancyAdvantagePerc(int strAccurancyAdvantage);
        double getSigStrAdvantageByPosStanding(double strAccurancyAdvantagePerc)const;
        double getSigStrAdvantageByPosGround(double strAccurancyAdvantagePerc)const;
        
        double getTakeDownAccurancyAdvantage(const AthleteInfoReader&opponent)const;
        double getTakeDownAccurancyAdvantagePerc(int takeDownAccurancyAdvantage);
        double getSubmissionAdvantage(double takeDownAccurancyAdvantagePerc);
        */
        //int getWinsSum()const;

        int getManAdvantage()const; 
        double getPower()const;
        double getEndurance(double sigStrAbsTotal)const;


        /*double getSigStrAbsorbedTotal()const;
        double getSigStrLandedTotal()const;
        
        double getSigStrAbsorbedTotal(double averageFightTimeAsMinuts)const;
        */

       double getSigStrLandedTotal(double averageFightTimeAsMinuts)const;
       double getSigStrAbsorbedTotal(const AthleteInfoReader&opponent)const;

       
        AthleteInfoReader()=default;
        AthleteInfoReader(const AthleteInfoReader&)=delete;
        AthleteInfoReader& operator=(const AthleteInfoReader&)=delete;
        AthleteInfoReader(AthleteInfoReader&&)noexcept=default;
        AthleteInfoReader& operator=(AthleteInfoReader&&)noexcept=default;
        ~AthleteInfoReader()=default;
    };
}

#endif // ! __KOAI_ATHLETE_INFO__ INCLUDED