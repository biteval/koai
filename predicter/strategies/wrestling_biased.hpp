#ifndef __KOAI_WRESTLING_BIASED__
#define __KOAI_WRESTLING_BIASED__

#include "../predict_strategy.hpp"
#include "fight_calc.hpp"


namespace strategy{
    class WrestlingBiased : public predict::PredictStrategy{
        FightCalc fightCalc;
        public:
        WrestlingBiased()=default;
        nlohmann::json predict(athlete_info::AthleteInfoReader&athlete1, athlete_info::AthleteInfoReader&athlete2);
        WrestlingBiased(const WrestlingBiased&)=delete;
        WrestlingBiased& operator=(const WrestlingBiased&)=delete;
        WrestlingBiased(WrestlingBiased&&)noexcept=default;
        WrestlingBiased& operator=(WrestlingBiased&&)noexcept=default;
        ~WrestlingBiased()=default;
    };
}


#endif // ! __KOAI_WRESTLING_BIASED__ INCLUDED