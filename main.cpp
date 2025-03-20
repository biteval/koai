#include "gui/web_server.hpp"

/**
* @author Mohamed Naamane (mrnmnwork@gmail.com)
*/

int main(){
   /*data_provider::DataProviderManager dataProvider;
   database_ctl::UfcDataBaseControllerConf ufcCtlConfigurer;
   database::Credentials cred;
   cred.username = std::getenv(koai::exported::CASS_USERNAME);
   cred.password = std::getenv(koai::exported::CASS_PASS);
   if(cred.username==nullptr || cred.password==nullptr){
       auto logger = dataProvider.getLogger();
       logger->add("CASS_USERNAME | CASS_PASS env varibales not exported.\n");
       logger->log();
       logger->clear();
   }else{
      std::string hosts ="127.0.0.1";
      dataProvider.configureDbController(ufcCtlConfigurer).
      setCredentials(std::move(hosts), std::move(cred));
   }

    //create the databse for all ufc athletes ,call createDatabase for the first time only to create a new database.
    dataProvider.createDatabase();
    //create the databse for ufc athletes with name john only
    //dataProvider.createDatabase("john");


    //update the database 
   //dataProvider.updateDatabase(); 

   //update the database for athelets with name john only
   //dataProvider.updateDatabase("john");

   */
   
   //start the crow server after creating the databse.
   webserver::WebServer koAiServer;
   koAiServer.listen();
   return 0;
}
