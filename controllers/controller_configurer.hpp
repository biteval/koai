#ifndef __KOAI_CONTROLLER_CONFIGURER__
#define __KOAI_CONTROLLER_CONFIGURER__

namespace database_ctl{
   template<typename Configurer>
   class ControllerConfigurer{
        public:
        void configure(auto&dbController){
            static_cast<Configurer&>(*this).configure(dbController);
        }
   };
}

#endif // ! __KOAI_CONTROLLER_CONFIGURER__ INCLUDED
