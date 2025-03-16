#ifndef __KOAI_STRIKING_BIASED__
#define __KOAI_STRIKING_BIASED__

#include "../predict_strategy.hpp"
#include "fight_calc.hpp"

namespace strategy{
    class StrikingBiased : public predict::PredictStrategy{
        FightCalc fightCalc;
        public:
        StrikingBiased()=default;
        nlohmann::json predict(athlete_info::AthleteInfoReader&athlete1, athlete_info::AthleteInfoReader&athlete2);
        StrikingBiased(const StrikingBiased&)=delete;
        StrikingBiased& operator=(const StrikingBiased&)=delete;
        StrikingBiased(StrikingBiased&&)noexcept=default;
        StrikingBiased& operator=(StrikingBiased&&)noexcept=default;
        ~StrikingBiased()=default;
    };
}

#endif // ! __KOAI_STRIKING_BIASED__ INCLUDED