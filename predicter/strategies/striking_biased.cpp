#include "striking_biased.hpp"


nlohmann::json strategy::StrikingBiased::predict(athlete_info::AthleteInfoReader&athlete1, athlete_info::AthleteInfoReader&athlete2){
    unsigned ath1Points{0};
    unsigned ath2Points{0}; 
    auto ath1StrAcc = athlete1.getStrikingAccurancy();
    auto ath2StrAcc = athlete2.getStrikingAccurancy();
    
    constexpr unsigned STR_ADV = 2;
    if(ath1StrAcc>ath2StrAcc){
      ath1Points+=STR_ADV;
    }else if(ath2StrAcc>ath1StrAcc){
      ath2Points+=STR_ADV;
    }
    auto ath1SigStrDefense = athlete1.getSigStrDefense();
    auto ath2SigStrDefense = athlete2.getSigStrDefense();
  
    if(ath1SigStrDefense>ath2SigStrDefense){
      ath1Points+=STR_ADV;
    }else if(ath2SigStrDefense>ath1SigStrDefense){
      ath2Points+=STR_ADV;
    }
  
    auto ath1KnockdownAvg = athlete1.getKnockdownAvg();
    auto ath2KnockdownAvg = athlete2.getKnockdownAvg();
  
    if(ath1KnockdownAvg>ath2KnockdownAvg){
      ath1Points+=STR_ADV;
    }else if(ath2KnockdownAvg>ath1KnockdownAvg){
      ath2Points+=STR_ADV;
    }
  
    auto ath1SigStrByTargetHead = athlete1.getSigStrByTargetHead();
    auto ath2SigStrByTargetHead = athlete2.getSigStrByTargetHead();
  
    if(ath1SigStrByTargetHead>ath2SigStrByTargetHead){
      ath1Points+=STR_ADV;
    }else if(ath2SigStrByTargetHead>ath1SigStrByTargetHead){
      ath2Points+=STR_ADV;
    }
    
    auto ath1WinByKoTko = athlete1.getWinByKoTko();
    auto ath2WinByKoTko = athlete2.getWinByKoTko();
  
    if(ath1WinByKoTko>ath2WinByKoTko){
      ath1Points+=STR_ADV+1;
    }else if(ath2WinByKoTko>ath1WinByKoTko){
      ath2Points+=STR_ADV+1;
    }
  
    double aht1SigStrAbsTotal =  athlete1.getSigStrAbsorbedTotal(athlete2);
    double aht2SigStrAbsTotal =  athlete2.getSigStrAbsorbedTotal(athlete1);
    
    //printf("aht1SigStrAbsTotal %d\n", int(aht1SigStrAbsTotal));
    //printf("aht2SigStrAbsTotal %d\n", int(aht2SigStrAbsTotal));
    
    if(aht1SigStrAbsTotal<aht2SigStrAbsTotal){
      ath1Points+=1;
    }else if(aht2SigStrAbsTotal<aht1SigStrAbsTotal){
      ath2Points+=1;
    }
     
    auto ath1WinByDec =  athlete1.getWinByDec();
    auto ath2WinByDec =  athlete2.getWinByDec();
    
    //printf("ath1Endurance %d\n", int(ath1WinByDec));
    //printf("ath2Endurance %d\n", int(ath2WinByDec));
    //higher win by dec indicates more endurance
    if(ath1WinByDec>ath2WinByDec){
      ath1Points+=STR_ADV+1;
    }else if(ath2WinByDec>ath1WinByDec){
      ath2Points+=STR_ADV+1;
    }
    
   
    //printf("ath1Points %d\n", int(ath1Points));
    //printf("ath2Points %d\n", int(ath2Points));
    //init the prediction result.
    nlohmann::json result{};
    result[predict::keys::IS_DRAW]=false;
  
    if(ath1Points>ath2Points){
      //calc the fight time.
      double ftime = fightCalc.calcFightTime(athlete1,athlete2);
      result[predict::keys::FIGHT_TIME] = ftime;
      result[predict::keys::WINNER] = athlete1.getFullNameWithNickName();
      result[predict::keys::DEFEATED] = athlete2.getFullName();
      result[predict::keys::WIN_BY]=fightCalc.calcWinByMethod(athlete1);
      result[predict::keys::WIN_POS]=fightCalc.calcWinByPos(athlete1);
  
    }else if(ath2Points>ath1Points){
      //calc the fight time.
      double ftime = fightCalc.calcFightTime(athlete2,athlete1);
      result[predict::keys::FIGHT_TIME] = ftime;
      result[predict::keys::WINNER] = athlete2.getFullNameWithNickName();
      result[predict::keys::DEFEATED] = athlete1.getFullName();
      result[predict::keys::WIN_BY]=fightCalc.calcWinByMethod(athlete2);
      result[predict::keys::WIN_POS]=fightCalc.calcWinByPos(athlete2);
  
    }else{//draw
      result[predict::keys::IS_DRAW]=true;
      result[predict::keys::WINNER] = athlete1.getFullNameWithNickName();
      result[predict::keys::DEFEATED] = athlete2.getFullNameWithNickName();
    }
    return result;
  }
  