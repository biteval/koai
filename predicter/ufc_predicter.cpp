#include "ufc_predicter.hpp"

predict::UfcPredicter::UfcPredicter(){
    currAlgorithm=std::make_unique<strategy::DefaultStrategy>();
    currStrategy=strategy_types::PREDICT_STRATEGY::DEFAULT;
    recentPredictions=nlohmann::json{};
}

void predict::UfcPredicter::addRecentPrediction(std::string pred){
   constexpr size_t MAX_SIZE = 10;
    if(recentPredictions.size()>=MAX_SIZE){
        recentPredictions.clear();
   }
   recentPredictions.push_back(std::move(pred));
}

nlohmann::json predict::UfcPredicter::getRecentPredictions(){
    std::shared_lock<std::shared_mutex>lock(mtx);
    return recentPredictions;
}

void predict::UfcPredicter::setStrategy(const strategy_types::PREDICT_STRATEGY strategy)const{
    using namespace strategy_types;
    if(currStrategy==strategy){
        return;
    }
    currStrategy = strategy;
    if (strategy==PREDICT_STRATEGY::DEFAULT){
        currAlgorithm = std::make_unique<strategy::DefaultStrategy>();
    }else if(strategy==PREDICT_STRATEGY::WRESTLING_BIASED){
        currAlgorithm = std::make_unique<strategy::WrestlingBiased>();
    }else{
        currAlgorithm = std::make_unique<strategy::DefaultStrategy>();
    }
}

 
nlohmann::json predict::UfcPredicter::predict(athlete_info::AthleteInfoReader&athlete1, athlete_info::AthleteInfoReader&athlete2){
    nlohmann::json algResponse{};
    switch(currStrategy){
        case strategy_types::PREDICT_STRATEGY::DEFAULT:
        algResponse = currAlgorithm->predict<strategy::DefaultStrategy>(athlete1,athlete2);
        break;
        case strategy_types::PREDICT_STRATEGY::WRESTLING_BIASED:
        algResponse = currAlgorithm->predict<strategy::WrestlingBiased>(athlete1,athlete2);
        break;
        
        case strategy_types::PREDICT_STRATEGY::STRIKING_BIASED:
        algResponse = currAlgorithm->predict<strategy::StrikingBiased>(athlete1,athlete2);
        break;

        default:
        break;
    }

    /*auto simpleRes = currAlgorithm->predict<strategy::DefaultStrategy>(athlete1,athlete2);
    auto wresRes = currAlgorithm->predict<strategy::WrestlingBiased>(athlete1,athlete2);
    auto srtRes = currAlgorithm->predict<strategy::StrikingBiased>(athlete1,athlete2);
    
    
    printf("winner1 %s\n", simpleRes[predict::keys::WINNER].get<std::string>().c_str());
    printf("winner2 %s\n", wresRes[predict::keys::WINNER].get<std::string>().c_str());
    printf("winner3 %s\n", srtRes[predict::keys::WINNER].get<std::string>().c_str());*/
    utils::JsonSearch jsonSearch;

    std::string winner = jsonSearch.readStr(algResponse,predict::keys::WINNER);
    std::string defeated = jsonSearch.readStr(algResponse,predict::keys::DEFEATED);

    std::string recentPred = winner + " vs " + defeated;
    addRecentPrediction(recentPred);
    double fightTime{};
    if(algResponse[predict::keys::FIGHT_TIME]!=nullptr){
        fightTime = algResponse[predict::keys::FIGHT_TIME];
    }

    std::stringstream predictionText;
    predictionText << std::fixed << std::setprecision(2);
   if(algResponse[predict::keys::IS_DRAW].get<bool>()){
     //draw
     predictionText<<"I predict the fight between " <<winner<<" and "<<defeated<<" will end in a Draw.";
     nlohmann::json response{};
     response["text"]=predictionText.str();
     return response;
   }

   std::string winBy = jsonSearch.readStr(algResponse,predict::keys::WIN_BY);
   std::string winPos = jsonSearch.readStr(algResponse,predict::keys::WIN_POS);
   
   winPos = winPos==predict::properties::WIN_POS_GROUND ? "on the " + winPos : winPos;
   predictionText<<"I predict "<<winner<<" will defeat "<<
   defeated<<" by "<<winBy<<" likely finishing the fight "<<winPos
   <<" with an estimated fight time of "<<fightTime<<" minutes.";
   nlohmann::json response{};
   response["text"]=predictionText.str();
   return response;
}
