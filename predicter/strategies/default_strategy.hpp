#ifndef  __KOAI_SIMPLE_STRATEGY__
#define  __KOAI_SIMPLE_STRATEGY__

#include "../predict_strategy.hpp"
#include "fight_calc.hpp"

/*
the DefaultStrategy class simulates the default fight between 2 fighters without 
any biase to fighting style or skill.
*/

namespace strategy{
    class DefaultStrategy : public predict::PredictStrategy{
        FightCalc fightCalc;
        public:
        DefaultStrategy()=default;
        //predict the fight result between 2 fighters  
        nlohmann::json predict(athlete_info::AthleteInfoReader&athlete1, athlete_info::AthleteInfoReader&athlete2);
        DefaultStrategy(const DefaultStrategy&)=delete;
        DefaultStrategy& operator=(const DefaultStrategy&)=delete;
        DefaultStrategy(DefaultStrategy&&)noexcept=default;
        DefaultStrategy& operator=(DefaultStrategy&&)noexcept=default;
        ~DefaultStrategy()=default;
    };
}


#endif // ! __KOAI_SIMPLE_STRATEGY__ INCLUDED.