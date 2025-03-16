#ifndef __KOAI_SCREEN_CLEANER__
#define __KOAI_SCREEN_CLEANER__


namespace screen_cleaner{
    class ScreenCleaner{
      public:
        ScreenCleaner()=default;
        ScreenCleaner(const ScreenCleaner&)=delete;
        ScreenCleaner operator=(const ScreenCleaner&)=delete;
        ScreenCleaner(ScreenCleaner&&)=delete;
        ScreenCleaner operator=(ScreenCleaner&&)=delete;
        ~ScreenCleaner()=default;
        static void clearScreen();
    };
}

#endif // ! __KOAI_SCREEN_CLEANER__ INCLUDED

