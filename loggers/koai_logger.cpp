#include "koai_logger.hpp"

logging::KoAiLogger::KoAiLogger():msgs(std::vector<std::string>{}){}


void logging::KoAiLogger::printMsgs(){
   for(size_t i = 0; i<msgs.size();++i){
    printf("%s",msgs[i].c_str());
   }
   //msgs.clear();
}


void logging::KoAiLogger::printMsgsBackward(){
  for(int i = static_cast<int>(msgs.size()-1); i>=0;--i){
    printf("%s",msgs[i].c_str());
   }
   //msgs.clear();
}


void logging::KoAiLogger::printMsg(LogMode&mode){
  if(msgs.empty()){
    return;
  } 
  if(mode==LogMode::FIRST){
    auto&msg = msgs.front();
    printf("%s",msg.c_str());
    //msgs.erase(msgs.begin());
   }
   else{
    auto&msg = msgs.back();
    printf("%s",msg.c_str());
    //msgs.pop_back();
   }
}


void logging::KoAiLogger::log(LogMode mode){
  switch (mode){
  case LogMode::FULL:
    printMsgs();
    break;
  case LogMode::BACKWARDS:
    printMsgsBackward();
    break;
  default:
    printMsg(mode);
    break;
  }
}


void logging::KoAiLogger::clear(LogMode mode){
   if(msgs.empty()){
    return;
   } 
   if(mode==LogMode::RECENT){
     msgs.pop_back();
    }else if(mode==LogMode::FIRST){
      msgs.erase(msgs.begin());
    }else msgs.clear();
}


logging::KoAiLogger::~KoAiLogger()=default;
