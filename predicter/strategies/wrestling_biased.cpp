#include "wrestling_biased.hpp"

nlohmann::json strategy::WrestlingBiased::predict(athlete_info::AthleteInfoReader&athlete1, athlete_info::AthleteInfoReader&athlete2){
    unsigned ath1Points{0};
    unsigned ath2Points{0};
    auto ath1TakeDownAcc = athlete1.getTakedownAccuracy();
    auto ath2TakeDownAcc = athlete2.getTakedownAccuracy();
    constexpr unsigned TAKE_DOWN_ADV = 3;
    if(ath1TakeDownAcc>ath2TakeDownAcc){
      ath1Points+=TAKE_DOWN_ADV;
    }else if(ath2TakeDownAcc>ath1TakeDownAcc){
      ath2Points+=TAKE_DOWN_ADV;
    }
  
   auto ath1SubmissionAvgPer15Min = athlete1.getSubmissionAvgPer15Min();
    auto ath2SubmissionAvgPer15Min = athlete2.getSubmissionAvgPer15Min();
    constexpr unsigned SUB_ADV = 4;
    if(ath1SubmissionAvgPer15Min>ath2SubmissionAvgPer15Min){
      ath1Points+=SUB_ADV;
    }else if(ath2SubmissionAvgPer15Min>ath1SubmissionAvgPer15Min){
      ath2Points+=SUB_ADV;
    }
  
    auto ath1TakeDownDefense = athlete1.getTakeDownDefense();
    auto ath2TakeDownDefense = athlete2.getTakeDownDefense();
  
    if(ath1TakeDownDefense>ath2TakeDownDefense){
      ath1Points+=TAKE_DOWN_ADV;
    }else if(ath2TakeDownDefense>ath1TakeDownDefense){
      ath2Points+=TAKE_DOWN_ADV;
    }
  
  
    auto ath1WinBySub = athlete1.getWinBySub();
    auto ath2WinBySub = athlete2.getWinBySub();
    
    if(ath1WinBySub>ath2WinBySub){
      ath1Points+=SUB_ADV;
    }else if(ath2WinBySub>ath1WinBySub){
      ath2Points+=SUB_ADV;
    }
    
  
    /*printf("ath1Points %d\n", int(ath1Points));
    printf("ath2Points %d\n", int(ath2Points));*/
  
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
  