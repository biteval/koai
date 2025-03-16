#ifndef __KOAI_PREDICT_RESULT__
#define __KOAI_PREDICT_RESULT__

namespace predict{
    namespace properties{
     constexpr auto WIN_POS_STANDING ="standing";
     constexpr auto WIN_POS_CLINCH   ="clinch";
     constexpr auto WIN_POS_GROUND = "ground";

     constexpr auto WIN_METHOD_KO = "KO/TKO";
     constexpr auto WIN_METHOD_DEC = "DECISION";
     constexpr auto WIN_METHOD_SUB = "SUBMISSION";
    }
    namespace keys{
        constexpr auto WINNER = "winner";
        constexpr auto DEFEATED = "defeated";
        constexpr auto WIN_BY ="winby";
        constexpr auto WIN_POS ="winpos";
        constexpr auto FIGHT_TIME = "ftime";
        constexpr auto IS_DRAW = "isdraw";
    }
}


#endif // ! __KOAI_PREDICT_RESULT__ INCLUDED