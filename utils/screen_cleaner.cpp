#include "screen_cleaner.hpp"
#include <cstdlib>


void screen_cleaner::ScreenCleaner::clearScreen(){
    #ifdef _WIN32
    std::system("cls");
    #else
    std::system("clear");
    #endif
}