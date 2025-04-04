#ifndef __KOAI_APP__
#define __KOAI_APP__
 
/**
* @brief KoAi class provides functionality to start the app
*/

#include "../controllers/ufc_db_ctl_conf.hpp"
#include "../managers/data_provider_manager.hpp"
#include "../predicter/ufc_predicter.hpp"


namespace koai{
    namespace exported{
        constexpr auto CASS_USERNAME ="CASS_USERNAME";
        constexpr auto CASS_PASS ="CASS_PASS";
    }
    class KoAi{
        database_ctl::UfcDataBaseControllerConf ufcCtlConfigurer;
        data_provider::DataProviderManager dataProvider;
        predict::UfcPredicter ufcPredicter;
        nlohmann::json getFighterBasicDetails(nlohmann::json&records);
        void initDataProvider();
        public:
        KoAi();
        nlohmann::json autoComplete(std::string athleteName);
        nlohmann::json predict(nlohmann::json&athlete1, nlohmann::json&athlete2);
        nlohmann::json getRecentPredictions();
        KoAi(const KoAi&)=delete;
        KoAi& operator=(const KoAi&)=delete;
        KoAi(KoAi&&)=delete;
        KoAi& operator=(KoAi&&)=delete;
        ~KoAi()=default;
    };
}


#endif // ! __KOAI_APP__ INCLUDED