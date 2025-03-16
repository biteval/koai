#ifndef __KOAI_CASSANDRA_WRAPPER__
#define __KOAI_CASSANDRA_WRAPPER__

#include "authenticator.hpp"
#include <vector>
#include <nlohmann/json.hpp>


namespace database{
    class Cassandra{
        CassFuture* connect_future;
        CassCluster* cluster;
        CassSession* session;
        std::vector<std::string>columnsTitles;
        const CassResult*getQueryResult(std::string&query)const; 
        std::string hosts;
        Credentials cred;
        public:
            Cassandra();
            Cassandra& clone();
            void setCredentials(std::string&& hosts,Credentials&&cred);
            Cassandra&conenct();
            Cassandra&setColumnsTitles(std::vector<std::string>&columnsTitles);
            nlohmann::json perform(std::string query)const;
            void destroy();
            Cassandra(const Cassandra&)=delete;
            Cassandra& operator=(const Cassandra&)=delete;
            Cassandra(Cassandra&&)=delete;
            Cassandra& operator=(Cassandra&&)=delete;
            ~Cassandra();
    };

}



#endif // ! __KOAI_CASSANDRA_WRAPPER__ INCLUDED