#ifndef __KOAI_PREDICT_ALGORITHM__
#define __KOAI_PREDICT_ALGORITHM__


namespace predict{
    class PredictStrategy{
      public:
      template <typename STRATEGY>
      auto predict(auto&athlete1, auto&athlete2){
         return static_cast<STRATEGY&>(*this).predict(athlete1,athlete2);
      };
    };
}

#endif //! __KOAI_PREDICT_ALGORITHM__ INCLUDED