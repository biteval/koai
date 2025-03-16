#include "default_strategy.hpp"

nlohmann::json strategy::DefaultStrategy::predict(athlete_info::AthleteInfoReader&athlete1, athlete_info::AthleteInfoReader&athlete2){
    unsigned ath1Points{0};
    unsigned ath2Points{0}; 
    auto ath1StrAcc = athlete1.getStrikingAccurancy();
    auto ath2StrAcc = athlete2.getStrikingAccurancy();
    
    constexpr unsigned STR_ADV = 3;
    if(ath1StrAcc>ath2StrAcc){
      ath1Points+=STR_ADV;
    }else if(ath2StrAcc>ath1StrAcc){
      ath2Points+=STR_ADV;
    }
     
    auto ath1TakeDownAcc = athlete1.getTakedownAccuracy();
    auto ath2TakeDownAcc = athlete2.getTakedownAccuracy();
    constexpr unsigned TAKE_DOWN_ADV = 3;
    if(ath1TakeDownAcc>ath2TakeDownAcc){
      ath1Points+=TAKE_DOWN_ADV;
    }else if(ath2TakeDownAcc>ath1TakeDownAcc){
      ath2Points+=TAKE_DOWN_ADV;
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
      ath1Points+=1;
    }else if(ath2KnockdownAvg>ath1KnockdownAvg){
      ath2Points+=1;
    }
  
  
    auto ath1SubmissionAvgPer15Min = athlete1.getSubmissionAvgPer15Min();
    auto ath2SubmissionAvgPer15Min = athlete2.getSubmissionAvgPer15Min();
    constexpr unsigned SUB_ADV = 8;
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
      ath1Points+=1;
    }else if(ath2WinByKoTko>ath1WinByKoTko){
      ath2Points+=1;
    }
  
    auto ath1WinBySub = athlete1.getWinBySub();
    auto ath2WinBySub = athlete2.getWinBySub();
    constexpr unsigned WIN_BY_SUB_ADV = 2;
    if(ath1WinBySub>ath2WinBySub){
      ath1Points+=WIN_BY_SUB_ADV;
    }else if(ath2WinBySub>ath1WinBySub){
      ath2Points+=WIN_BY_SUB_ADV;
    }
  
  
    auto ath1WinByDec = athlete1.getWinByDec();
    auto ath2WinByDec = athlete2.getWinByDec();
  
    if(ath1WinByDec>ath2WinByDec){
      ath1Points+=1;
    }else if(ath2WinByDec>ath1WinByDec){
      ath2Points+=1;
    }

    //calc total wins
     unsigned ath1TotalWins = ath1WinByKoTko+ ath1WinBySub+ath1WinByDec;
     unsigned ath2TotalWins = ath2WinByKoTko+ ath2WinBySub+ath2WinByDec;
     constexpr unsigned EXPERIENCE_ADV = 3;
     if(ath1TotalWins>ath2TotalWins){
      ath1Points+=EXPERIENCE_ADV;
    }else if(ath2TotalWins>ath1TotalWins){
      ath2Points+=EXPERIENCE_ADV;
    }

    auto ath1SigStrByPosGround = athlete1.getSigStrByPosGround();
    auto ath2SigStrByPosGround = athlete2.getSigStrByPosGround();
  
    if(ath1SigStrByPosGround>ath2SigStrByPosGround){
      ath1Points+=STR_ADV;
    }else if(ath2SigStrByPosGround>ath1SigStrByPosGround){
      ath2Points+=STR_ADV;
    }
  
    auto ath1Age = athlete1.getAge();
    auto ath2Age = athlete2.getAge();
  
    if(ath1Age>ath2Age){
      ath2Points+=(ath1Age-ath2Age);
    }else if(ath2Age>ath1Age){
      ath1Points+=(ath2Age-ath1Age);
    }
    
  
    auto ath1Status = athlete1.getStatus();
    auto ath2Status = athlete2.getStatus();
    const std::string active="active";
    constexpr unsigned STATUS_ADV = 2;
    if(ath1Status==active && ath2Status!=active){
      ath1Points+=STATUS_ADV;
    }else if(ath2Status==active && ath1Status!=active){
      ath2Points+=STATUS_ADV;
    }
  
  
    auto ath1Gender = athlete1.getGender();
    auto ath2Gender = athlete2.getGender();
    const std::string male="male";
    //(STR_ADV+SUB_ADV+TAKE_DOWN_ADV)*2
    constexpr unsigned MALE_ADV = 18;
    if(ath1Gender==male && ath2Gender!=male){
      ath1Points+=MALE_ADV;
    }else if(ath2Gender==male && ath1Gender!=male){
      ath2Points+=MALE_ADV;
    }
  
  
    double aht1SigStrAbsTotal =  athlete1.getSigStrAbsorbedTotal(athlete2);
    double aht2SigStrAbsTotal =  athlete2.getSigStrAbsorbedTotal(athlete1);
    
    
    if(aht1SigStrAbsTotal<aht2SigStrAbsTotal){
      ath1Points+=1;
    }else if(aht2SigStrAbsTotal<aht1SigStrAbsTotal){
      ath2Points+=1;
    }
    
    auto ath1Endurance =  athlete1.getEndurance(aht1SigStrAbsTotal+ath2Points);
    auto ath2Endurance =  athlete2.getEndurance(aht2SigStrAbsTotal+ath1Points);
    
    constexpr unsigned IND_ADV= 3;
    if(ath1Endurance>ath2Endurance){
      ath1Points+=IND_ADV;
    }else if(ath2Endurance>ath1Endurance){
      ath2Points+=IND_ADV;
    }
  
    
    /*double ath1TakeDownAccAdv = athlete1.getTakeDownAccurancyAdvantage(athlete2);
    double ath2TakeDownAccAdv = athlete2.getTakeDownAccurancyAdvantage(athlete1);
  
    std::cout<<"ath1TakeDownAccAdv "<< ath1TakeDownAccAdv <<"\n";
    std::cout<<"ath2TakeDownAccAdv "<< ath2TakeDownAccAdv <<"\n";
    
    if(ath1TakeDownAccAdv>ath2TakeDownAccAdv){
      ath1Points+=1;
    }else if(ath2TakeDownAccAdv>ath1TakeDownAccAdv){
      ath2Points+=1;
    }*/
  
    //printf("ath1Points %d\n", int(ath1Points));
    //printf("ath2Points %d\n", int(ath2Points));
    //init the prediction result.
    nlohmann::json result{};
    result[predict::keys::IS_DRAW]=false;
  
    if(ath1Points>ath2Points){
      result[predict::keys::WINNER] = athlete1.getFullNameWithNickName();
      result[predict::keys::DEFEATED] = athlete2.getFullName();
      //calc the fight time.
      double ftime = fightCalc.calcFightTime(athlete1,athlete2);
      result[predict::keys::FIGHT_TIME] = ftime;
      result[predict::keys::WIN_BY]=fightCalc.calcWinByMethod(athlete1);
      result[predict::keys::WIN_POS]=fightCalc.calcWinByPos(athlete1);
    }else if(ath2Points>ath1Points){
      result[predict::keys::WINNER] = athlete2.getFullNameWithNickName();
      result[predict::keys::DEFEATED] = athlete1.getFullName();
       //calc the fight time.
       double ftime = fightCalc.calcFightTime(athlete2,athlete1);
       result[predict::keys::FIGHT_TIME] = ftime;
      result[predict::keys::WIN_BY]=fightCalc.calcWinByMethod(athlete2);
      result[predict::keys::WIN_POS]=fightCalc.calcWinByPos(athlete2);  
     }else{ //draw
      result[predict::keys::IS_DRAW]=true;
      result[predict::keys::WINNER] = athlete1.getFullNameWithNickName();
      result[predict::keys::DEFEATED] = athlete2.getFullNameWithNickName();
    }
   return result;
  }
  