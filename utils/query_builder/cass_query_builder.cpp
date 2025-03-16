#include "cass_query_builder.hpp"

database_utils::CassQueryBuilder::CassQueryBuilder(){
    currQuery=std::string{};
}


void database_utils::CassQueryBuilder::setConf(qry_builder_conf::CassQueryBuilderConf &&queryConf){
    this->queryConf=std::move(queryConf);
}


std::string database_utils::CassQueryBuilder ::getQualifiedTableName(){
    if(queryConf.keySpace.empty() || 
    queryConf.tableName.empty()){
    return std::string{};
    }
    std::string fqtn =queryConf.keySpace +"."+queryConf.tableName;
    return fqtn; 
}


database_utils::CassQueryBuilder & database_utils::CassQueryBuilder ::withTitles(){
    std::string fqtn = getQualifiedTableName();
    if(fqtn.empty()){
     //log error to create the full qualified table name
    }
    currQuery +="CREATE TABLE IF NOT EXISTS " + fqtn +"(\n";
    for(size_t i = 0; i< queryConf.titles.size();++i){
        currQuery+=queryConf.titles[i]+" TEXT,\n";
    }
    return *this;
}


std::vector<std::string> database_utils::CassQueryBuilder ::getTableTitles(){
  return queryConf.titles;
}



database_utils::CassQueryBuilder & database_utils::CassQueryBuilder ::withTablePrimaryKey(){
    std::string Pkeys = std::string{};
    for(size_t i=0; i<queryConf.primaryKey.size()-1;++i){
        Pkeys+=queryConf.primaryKey[i]+", ";
    }
    Pkeys+=queryConf.primaryKey[queryConf.primaryKey.size()-1];
    currQuery+="PRIMARY KEY ("+Pkeys+")\n)";
    return *this;
}


database_utils::CassQueryBuilder & database_utils::CassQueryBuilder ::setClusteringOrderBy(std::string title, ORDER by){
   bool keyFound=false;
    for(auto&ky:queryConf.primaryKey){
    if(ky==title){
        keyFound=true;
        break;
    }
   }
   if(!keyFound){
    printf("error the clustering not found it the primary key vector");
    //error the clustering not found it the primary key vector
   }
   std::string cmd="WITH CLUSTERING ORDER BY (";
   auto pos = currQuery.find(cmd);
   if(pos==std::string::npos){
     currQuery+=cmd;
   }
    //if the query already contains  ASC or DESC keywords add ,
    if(currQuery.find("ASC")!=std::string::npos ||
    currQuery.find("DESC")!=std::string::npos){
        currQuery+=",";
    }

    currQuery+=title+" ";
   
    switch(by){
        case ORDER::ASC:
        currQuery+="ASC";
        break;
        case  ORDER::DESC:
        currQuery+="DESC";
        break;
        default:
        currQuery+=")";
        break;
    }
  return *this;
}

std::string database_utils::CassQueryBuilder ::getCreateTableQuery(){
    if(currQuery.empty()){
        return std::string{};
    }
    std::string createTableQry = currQuery+")";
    return createTableQry;
}


std::string database_utils::CassQueryBuilder ::getSelectQuery(nlohmann::json&record){
  std::string fqtn = getQualifiedTableName();
  std::string selectQuery ="SELECT * FROM "+ fqtn+" WHERE ";
  //add primary key to the cmd
  for(size_t i =0;i<this->queryConf.primaryKey.size()-1;++i){
    std::string currKey = this->queryConf.primaryKey[i];
    std::string currKeyValue = record.value(currKey, std::string{});
    std::string qryPortion = currKey+"='"+currKeyValue+"' AND ";
    selectQuery+=qryPortion;
    
  }
  std::string currKey = this->queryConf.primaryKey[this->queryConf.primaryKey.size()-1];
  std::string currKeyValue = record.value(currKey, std::string{});
  std::string qryPortion = currKey+"='"+currKeyValue+"'";
  selectQuery+=qryPortion;
  return selectQuery;
}

std::string database_utils::CassQueryBuilder::getUniqueSelectQuery(nlohmann::json&record){
    std::string fqtn = getQualifiedTableName();
    std::string selectQuery ="SELECT * FROM "+ fqtn+" WHERE ";
    size_t i{0};
    for(auto[key,val] :record.items()){
        std::string currKey = key;
        std::string currValue = val;
        if(i==record.size()-1){
            std::string qryPortion = currKey+"='"+currValue+"'";
            selectQuery+=qryPortion;
            break;
        }
        std::string qryPortion = currKey+"='"+currValue+"' AND ";
        selectQuery+=qryPortion;
        ++i;
    }
   printf("unique query = %s\n",selectQuery.c_str());
   return selectQuery;
}

std::string database_utils::CassQueryBuilder ::getSelectQuery(std::string&key){
    std::string fqtn = getQualifiedTableName();
    std::string selectQuery ="SELECT * FROM "+ fqtn+" WHERE ";
    std::string firstKey = this->queryConf.primaryKey[0];
    selectQuery+=firstKey;
    selectQuery+="='";
    selectQuery+=key;
    selectQuery+="'";
    return selectQuery;
}



std::string database_utils::CassQueryBuilder ::getRemoveQuery(nlohmann::json&record){
    if(record==nullptr){
        return std::string{};
    }
    std::string fqtn = getQualifiedTableName();
    std::string removeQuery ="DELETE FROM  "+ fqtn+" WHERE ";
    //add primary key to the cmd
    for(size_t i =0;i<this->queryConf.primaryKey.size()-1;++i){
      std::string currKey = this->queryConf.primaryKey[i];
      std::string currKeyValue = record.value(currKey,std::string{});
      std::string qryPortion=currKey+"='"+currKeyValue+"' AND ";    
      removeQuery+= qryPortion;
      
    }
    std::string currKey = this->queryConf.primaryKey[this->queryConf.primaryKey.size()-1];
    std::string currKeyValue = record.value(currKey,std::string{});
    std::string qtyPortion = currKey+"='"+currKeyValue+"'";
    removeQuery+= qtyPortion;
    return removeQuery;
}

std::string database_utils::CassQueryBuilder::getCreateKeySpaceQuery(){
    std::string qry = "CREATE KEYSPACE IF NOT EXISTS " + queryConf.keySpace +" WITH replication = {'class':'"+queryConf.strategy+"', 'replication_factor':"+queryConf.replicationFactor+"}";
    return qry;
}

std::string database_utils::CassQueryBuilder ::getChangeClusterNameQuery(){
  std::string qry = "UPDATE system.local SET cluster_name = '"+queryConf.clusterName+"' where key='local';";
  return qry;
}


std::string database_utils::CassQueryBuilder ::getInsertQuery(nlohmann::json&record){
   std::string fqtn = getQualifiedTableName();
   if(fqtn.empty()){
    return std::string{};
   }
   std::string qty = "insert into "+fqtn+" (";
   std::string titles=std::string{};
   std::string values=std::string{};
   for (auto& [key, value] : record.items()) {
    std::string keyStr = key;
    std::string valStr = value;
    if(!titles.empty()){
        titles+=", ";
    }
    if(!values.empty()){
        values+=",";
    }
    titles+=keyStr;
    values+="'"+valStr+"'";
   }
   //remove additionall commas at the end
 
   qty+=titles+") values("+values+")";
   //printf("curr query = %s\n", qty.c_str());
   return qty;
}


void database_utils::CassQueryBuilder::clearQueries(){
    currQuery.clear();
    //queryStack.clear();
}


database_utils::CassQueryBuilder::~CassQueryBuilder(){}
