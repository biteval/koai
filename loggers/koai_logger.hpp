#ifndef __KOAI_LOGGER__
#define __KOAI_LOGGER__

#include "logger_modes.hpp"
#include <vector>
#include <sstream>
#include <iomanip>

namespace logging{
    class KoAiLogger{
      std::vector<std::string>msgs;
      //helper function to print all msgs all/backward
      void printMsgs();
      //helper function to print all msgs from backward
      void printMsgsBackward();
      void printMsg(LogMode&mode);
      public:
      KoAiLogger();
      KoAiLogger(const KoAiLogger&)=delete;
      KoAiLogger& operator=(const KoAiLogger&)=delete;
      KoAiLogger(KoAiLogger&&)noexcept=default;
      KoAiLogger& operator=(KoAiLogger&&)noexcept=default;
      template<typename ...Msgs>
      void add(Msgs&&...msg);
      void log(LogMode mode=LogMode::RECENT);
      void clear(LogMode mode=LogMode::RECENT);
      ~KoAiLogger();
    };
} // namespace logger

template<typename ...Msgs>
void logging::KoAiLogger::add(Msgs&&...msg){
  std::stringstream ss;
  ss << std::fixed << std::setprecision(2);
  (ss << ... << std::forward<Msgs>(msg));
  msgs.push_back(ss.str());
}

#endif // ! __KOAI_LOGGER__ INCLUDED