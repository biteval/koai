#include "athlete_info.hpp"

double athlete_info::AthleteInfoReader::strToDouble(std::string& val)const{
    try{
        double res = std::stod(val);
        return res;
     }catch(...){}
     return 0.;
}

int athlete_info::AthleteInfoReader::strToInt(std::string& val)const{
    try{
        int res = std::stoi(val);
        return res;
     }catch(...){}
     return 0;
}

size_t athlete_info::AthleteInfoReader::recordSize()const{
    return record.size();
}
 
 athlete_info::AthleteInfoReader::AthleteInfoReader(nlohmann::json&&record){
    this->record=std::move(record);
 }
 
 std::string athlete_info::AthleteInfoReader::getFirstName()const{
     return jsonSearch.readStr(record,database_titles::FIRST_NAME_KEY);
 }
 
 std::string athlete_info::AthleteInfoReader::getLastName()const{
     return jsonSearch.readStr(record,database_titles::LAST_NAME_KEY);
 }
 
 std::string athlete_info::AthleteInfoReader::getDivision()const{
     return jsonSearch.readStr(record,database_titles::DIVISION_KEY);
 }
 
 std::string athlete_info::AthleteInfoReader::getNickName()const{
     return jsonSearch.readStr(record,database_titles::NICKNAME_KEY);
 }
 
 int athlete_info::AthleteInfoReader::getStrikingAccurancy()const{
     std::string sastr = record[database_titles::STRIKING_ACCURACY_KEY];
     return strToInt(sastr);
 }
 
 
 int athlete_info::AthleteInfoReader::getTakedownAccuracy()const{
     std::string tkdAcc = record[database_titles::TAKEDOWN_ACCURACY_KEY];
     return strToInt(tkdAcc);
 }
 
 double athlete_info::AthleteInfoReader::getSigStrLandedPerMin()const{
     std::string ssland = record[database_titles::SIG_STR_LANDED_PER_MIN_KEY];
     double res = strToDouble(ssland);
     return res;
 }
 
 double athlete_info::AthleteInfoReader::getSigStrAbsorbedPerMin()const{
     std::string ssAbsor = record[database_titles::SIG_STR_ABSORBED_PER_MIN_KEY];
     return strToDouble(ssAbsor);
 }
 
 double athlete_info::AthleteInfoReader::getTakedownAvgPer15Min()const{
     std::string tkdavg = record[database_titles::TAKEDOWN_AVG_PER_15_MIN_KEY];
     return strToDouble(tkdavg);
 }
 
 
 // percentage
 int athlete_info::AthleteInfoReader::getSigStrDefense()const{
     std::string val = record[database_titles::SIG_STR_DEFENSE_KEY];
     return strToInt(val);
 }
 
 double athlete_info::AthleteInfoReader::getKnockdownAvg()const{
     std::string val = record[database_titles::KNOCKDOWN_AVG_KEY];
     return strToDouble(val);
 }
 
 double athlete_info::AthleteInfoReader::getSubmissionAvgPer15Min()const{
     std::string val = record[database_titles::SUBMISSION_AVG_KEY];
     double res = strToDouble(val);
     return res;
 }
 
 // percentage
 int athlete_info::AthleteInfoReader::getTakeDownDefense()const{
     std::string val = record[database_titles::TAKEDOWN_DEFENSE_KEY];
     return strToInt(val);
 }
 
 double athlete_info::AthleteInfoReader::getAverageFightTimeAsMinuts()const{
     std::string val = record[database_titles::AVERAGE_FIGHT_TIME_AS_MINUTES_KEY];
     return strToDouble(val);
 }
 
 int athlete_info::AthleteInfoReader::getSigStrByPosStanding()const{
     std::string val = record[database_titles::SIG_STR_BY_POS_STANDING_KEY];
     return strToInt(val);
 }
 
 int athlete_info::AthleteInfoReader::getSigStrByPosClinch()const{
     std::string val = record[database_titles::SIG_STR_BY_POS_CLINCH_KEY];
     return strToInt(val);
 }
 
 int athlete_info::AthleteInfoReader::getSigStrByPosGround()const{
     std::string val = record[database_titles::SIG_STR_BY_POS_GROUND_KEY];
     return strToInt(val);
 }
 
 int athlete_info::AthleteInfoReader::getSigStrByTargetHead()const{
     std::string val = record[database_titles::SIG_STR_BY_TARGET_HEAD_KEY];
     return strToInt(val);
 }
 
 int athlete_info::AthleteInfoReader::getSigStrByTargetBody()const{
     std::string val = record[database_titles::SIG_STR_BY_TARGET_BODY_KEY];
     return strToInt(val);
 }
 
 int athlete_info::AthleteInfoReader::getSigStrByTargetLeg()const{
     std::string val = record[database_titles::SIG_STR_BY_TARGET_LEG_KEY];
     return strToInt(val);
 }
 
 int athlete_info::AthleteInfoReader::getWinByKoTko()const{
     std::string val = record[database_titles::WIN_BY_KO_TKO_KEY];
     return strToInt(val);
 }
 
 int athlete_info::AthleteInfoReader::getWinByDec()const{
     std::string val = record[database_titles::WIN_BY_DEC_KEY];
     return strToInt(val);
 }
 
 int athlete_info::AthleteInfoReader::getWinBySub()const{
     std::string val = record[database_titles::WIN_BY_SUB_KEY];
     return strToInt(val);
 }
 
 std::string athlete_info::AthleteInfoReader::getStatus()const{
     return jsonSearch.readStr(record,database_titles::STATUS_KEY);
 }
 
 std::string athlete_info::AthleteInfoReader::getFightingStyle()const{
     return jsonSearch.readStr(record,database_titles::FIGHTING_STYLE_KEY);
 }
 
 unsigned short athlete_info::AthleteInfoReader::getAge()const{
     std::string val = record[database_titles::AGE_KEY];
     auto intVal = strToInt(val);
     return static_cast<unsigned short>(intVal);
 }
 
 double athlete_info::AthleteInfoReader::getHeight()const{
     std::string val = record[database_titles::HEIGHT_DB_KEY];
     return strToDouble(val);
 }
 
 double athlete_info::AthleteInfoReader::getWeight()const{
     std::string val = record[database_titles::WEIGHT_DB_KEY];
     return strToDouble(val);
 }
 
 std::string athlete_info::AthleteInfoReader::getGender()const{
     return jsonSearch.readStr(record,database_titles::GENDER_KEY);
 }
 
 std::string athlete_info::AthleteInfoReader::getWeightClass()const{
     return jsonSearch.readStr(record,database_titles::WEIGHT_CLASS_DB_KEY);
 }
 
 std::string athlete_info::AthleteInfoReader::getImageUrl()const{
     return jsonSearch.readStr(record,database_titles::IMAGE_URL_DB_KEY);
 }
 
 std::string athlete_info::AthleteInfoReader::getFullNameWithNickName()const{
     std::string nickName = getNickName();
     nickName = nickName.empty() ? " " : " ("+nickName+") ";
     return getFirstName() + nickName + getLastName();
 }
 
 std::string athlete_info::AthleteInfoReader::getFullName()const{
     return getFirstName() + " " + getLastName();
 }
 
 
 /*int athlete_info::AthleteInfoReader::getStrAccurancyAdvantage(const AthleteInfoReader&opponent)const{
    return getStrikingAccurancy() - opponent.getSigStrDefense();
 }
 
 double athlete_info::AthleteInfoReader::getStrAccurancyAdvantagePerc(int strAccurancyAdvantage){
    return strAccurancyAdvantage / 100.;
 }
 
 double athlete_info::AthleteInfoReader::getSigStrAdvantageByPosStanding(double strAccurancyAdvantagePerc)const{
     return getSigStrByPosStanding() * strAccurancyAdvantagePerc;
 }
 
 double athlete_info::AthleteInfoReader::getSigStrAdvantageByPosGround(double strAccurancyAdvantagePerc)const{
    return getSigStrByPosGround() * strAccurancyAdvantagePerc;
 }
 
 double athlete_info::AthleteInfoReader::getTakeDownAccurancyAdvantage(const AthleteInfoReader&opponent)const{
    return getTakedownAccuracy() - opponent.getTakeDownDefense();
 }
 
 double athlete_info::AthleteInfoReader::getTakeDownAccurancyAdvantagePerc(int takeDownAccurancyAdvantage){
    return takeDownAccurancyAdvantage / 100.;
 }
 
 double athlete_info::AthleteInfoReader::getSubmissionAdvantage(double takeDownAccurancyAdvantagePerc){
    return getSubmissionAvgPer15Min() * takeDownAccurancyAdvantagePerc;
 }*/
 
 int athlete_info::AthleteInfoReader::getManAdvantage()const{
    const std::string male = "male";
    if(getGender()==male){
     return 5;
    }
    return 1;
 }
 
 
 /*int athlete_info::AthleteInfoReader::getWinsSum()const{
     return getWinBySub() + getWinByDec() + getWinByKoTko();
 }
 */
 
 
 double athlete_info::AthleteInfoReader::getPower()const{
    double weight = getWeight();
    long double weightPower = weight*weight;
    double heightmulAge = getHeight() * getAge();
    return weightPower / heightmulAge;
 }
 
 /*double athlete_info::AthleteInfoReader::getSigStrAbsorbedTotal()const{
    return getSigStrAbsorbedPerMin() * getAverageFightTimeAsMinuts();
 } 
 
 double athlete_info::AthleteInfoReader::getSigStrLandedTotal()const{
     return getSigStrLandedPerMin() * getAverageFightTimeAsMinuts();
 }
 
 
 double athlete_info::AthleteInfoReader::getSigStrAbsorbedTotal(double averageFightTimeAsMinuts)const{
     return getSigStrAbsorbedPerMin() * averageFightTimeAsMinuts;
 }
 */

 double athlete_info::AthleteInfoReader::getSigStrLandedTotal(double averageFightTimeAsMinuts)const{
    return getSigStrLandedPerMin() * averageFightTimeAsMinuts;
 }


 double athlete_info::AthleteInfoReader::getSigStrAbsorbedTotal(const AthleteInfoReader&opponent)const{
    return opponent.getSigStrLandedTotal(getAverageFightTimeAsMinuts());
 }

 double athlete_info::AthleteInfoReader::getEndurance(double totalMetrixs)const{
     double power = getPower();
     double manAdv = getManAdvantage();
     return power / (manAdv* totalMetrixs);
 }