#ifndef __KOAI_STRING_MANIPULATOR__
#define __KOAI_STRING_MANIPULATOR__

namespace utils{
    template <class Service>
    class StringManipulator{
      public:
      void normalizeText(auto&str){
         static_cast<Service&>(*this).normalizeText(str);
      }
    };
}

#endif // ! __KOAI_STRING_MANIPULATOR__ INCLUDED