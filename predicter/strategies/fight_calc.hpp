#ifndef __KOAI_FIGHT_CALC__
#define __KOAI_FIGHT_CALC__

#include "../athlete_info.hpp"
#include "../predict_result.hpp"


namespace strategy{
   class FightCalc{
    public:
   FightCalc()=default;
   double calcFightTime(athlete_info::AthleteInfoReader&winner, athlete_info::AthleteInfoReader&defeated);
   std::string calcWinByMethod(athlete_info::AthleteInfoReader&winner);
   std::string calcWinByPos(athlete_info::AthleteInfoReader&winner);
   FightCalc(FightCalc&)=delete;
   FightCalc& operator=(const FightCalc&)=delete;
   FightCalc(FightCalc&&)noexcept=default;
   FightCalc& operator=(FightCalc&&)noexcept=default;
  ~FightCalc()=default;
   };
 
}



#endif // ! __KOAI_FIGHT_CALC__ INCLUDED