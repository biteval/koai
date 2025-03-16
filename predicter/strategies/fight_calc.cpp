#include "fight_calc.hpp"

double strategy::FightCalc::calcFightTime(athlete_info::AthleteInfoReader&winner, athlete_info::AthleteInfoReader&defeated){
    double winnerFtime = winner.getAverageFightTimeAsMinuts();
    double defeatedFtime = defeated.getAverageFightTimeAsMinuts();
    double diff = (winnerFtime-defeatedFtime);
    diff=std::abs(diff);
    //printf("diff: %f\n", diff); 
    //auto defTimeMean = (defeatedFtime + diff) / 2;
    return  winnerFtime + diff/winnerFtime;
 }
 
 
 std::string strategy::FightCalc::calcWinByMethod(athlete_info::AthleteInfoReader&winner){
    int koTko = winner.getWinByKoTko();
    int dec = winner.getWinByDec();
    int sub = winner.getWinBySub();
    std::string winByKo = std::string(predict::properties::WIN_METHOD_KO);
    std::string winBySub = std::string(predict::properties::WIN_METHOD_SUB);
    std::string winByDec = std::string(predict::properties::WIN_METHOD_DEC);
    
    if(koTko>dec && koTko > sub){
     return winByKo;
    }else if(dec>koTko && dec > sub){
     return winByDec;
    }
    return winBySub;
 } 
 
 
 std::string strategy::FightCalc::calcWinByPos(athlete_info::AthleteInfoReader&winner){
     int standing = winner.getSigStrByPosStanding();
     int ground = winner.getSigStrByPosGround();
     std::string standingPos = std::string(predict::properties::WIN_POS_STANDING);
     std::string groundPos = std::string(predict::properties::WIN_POS_GROUND);
     if(standing>ground){
         return standingPos;
     }
     return groundPos;
 }
 