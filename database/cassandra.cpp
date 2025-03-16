#include "cassandra.hpp"

database::Cassandra::Cassandra(){
    hosts= std::string{};
    cred=Credentials{};
    connect_future=nullptr;
    cluster=cass_cluster_new();
    session=cass_session_new();
    columnsTitles=std::vector<std::string>{};
}


void database::Cassandra::setCredentials(std::string&& hosts,Credentials&&cred){
  this->hosts=std::move(hosts);
  this->cred=std::move(cred);
}


database::Cassandra&database::Cassandra::clone(){
  return *this;
}


database::Cassandra& database::Cassandra::conenct(){
  /* Add contact points */
  cass_cluster_set_contact_points(cluster, hosts.c_str());
  
  if(cred.isProvided()){
    Authenticator auth;
    auth.authenticate(cluster, cred);
   }

  /* Provide the cluster object as configuration to connect the session */
  connect_future = cass_session_connect(session, cluster);
  cass_future_wait(connect_future);

  if (cass_future_error_code(connect_future) == CASS_OK) {
    printf("Databse: connected.\n");
  }else{
     /* Handle error */
     const char* message{};
     size_t message_length{};
     cass_future_error_message(connect_future, &message, &message_length);
     printf("Unable to connect: %s\n", message);
  }
  return *this;
}


const CassResult* database::Cassandra::getQueryResult(std::string&query)const{
    if (cass_future_error_code(connect_future) == CASS_OK) {
        //CassFuture* close_future = NULL;
        CassStatement* statement = cass_statement_new(query.c_str(), 0);
        CassFuture* result_future = cass_session_execute(session, statement);
        
        if (cass_future_error_code(result_future) == CASS_OK) {
            const CassResult* result = cass_future_get_result(result_future);
            cass_future_free(result_future);
            cass_statement_free(statement);
            return result;
        }else{
            /* Handle error */
            const char* message;
            size_t message_length;
            cass_future_error_message(result_future, &message, &message_length);
            printf("Database: unable to run query, %s,%s\n",message,query.c_str());
            cass_future_free(result_future);
            cass_statement_free(statement);
            //printf("failed quety = %s\n",query.c_str());     
        }
    }else{
        const char* message;
        size_t message_length;
        cass_future_error_message(connect_future, &message, &message_length);
        printf("Database: Unable to connect,%s\n", message);
    }
    
    const CassResult* result=nullptr;
    return result;
}


database::Cassandra& database::Cassandra::setColumnsTitles(std::vector<std::string>&columnsTitles){
  this->columnsTitles=columnsTitles;
  return *this;
}

nlohmann::json database::Cassandra::perform(std::string query) const {
    if (query.empty()) {
        return nlohmann::json::array();
    }

    const CassResult* result = getQueryResult(query);
    if (result == nullptr) {
        return nlohmann::json::array();
    }

    nlohmann::json result_json = nlohmann::json::array();
    CassIterator* iterator = cass_iterator_from_result(result);

    while (cass_iterator_next(iterator)) {
        const CassRow* row = cass_iterator_get_row(iterator);
        nlohmann::json row_json{};

        for (const auto& cTitle : columnsTitles) {
            const CassValue* val = cass_row_get_column_by_name(row, cTitle.c_str());

            // Ensure val is valid before accessing it
            if (val == nullptr) {
                row_json[cTitle] = "";
                continue;
            }

            const char* output{};
            size_t outputSize{};
            
            if (cass_value_get_string(val, &output, &outputSize) == CASS_OK) {
                // Use explicit size to avoid overflow issues
                std::string safe_val(output, outputSize);
                row_json[cTitle] = std::move(safe_val);
            } else {
                row_json[cTitle] = "";
            }
        }

        result_json.push_back(row_json);
    }

    cass_iterator_free(iterator);
    cass_result_free(result);

    return result_json;
}

void database::Cassandra::destroy(){
    if (connect_future) {
        cass_future_free(connect_future);
    }
    if (session) {
        cass_session_free(session);
    }
    if (cluster) {
        cass_cluster_free(cluster);
    }
}


database::Cassandra::~Cassandra(){
    destroy();
}
