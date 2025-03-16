#ifndef __KOAI_UFC_PREDICTER__
#define __KOAI_UFC_PREDICTER__

#include "strategy_types.hpp"
#include "strategies/default_strategy.hpp"
#include "strategies/wrestling_biased.hpp"
#include "strategies/striking_biased.hpp"
#include <sstream>
#include <iomanip>
#include <memory>
#include <shared_mutex>


namespace predict{
    //strategy pattern class
    class UfcPredicter{
        mutable std::unique_ptr<predict::PredictStrategy>currAlgorithm;
        //mutable predict::PredictStrategy<STRATEGY>currAlgorithm;
        mutable strategy_types::PREDICT_STRATEGY currStrategy;
        void addRecentPrediction(std::string pred);
        mutable nlohmann::json recentPredictions;
        std::shared_mutex mtx;
        public:
        UfcPredicter();
        void setStrategy(const strategy_types::PREDICT_STRATEGY strategy)const;
        nlohmann::json predict(athlete_info::AthleteInfoReader&athlete1, athlete_info::AthleteInfoReader&athlete2);
        nlohmann::json getRecentPredictions();
        UfcPredicter(const UfcPredicter&)=delete;
        UfcPredicter& operator=(const UfcPredicter&)=delete;
        UfcPredicter(UfcPredicter&&)=delete;
        UfcPredicter& operator=(UfcPredicter&&)=delete;
    };
}

#endif // ! __KOAI_UFC_PREDICTER__ INCLUDED