# KoAI

<p align="center">
  <img src="https://github.com/biteval/koai/blob/main/static/screen_shot.png" alt="KoAi">
</p>

KoAi is a predictive platform that determines the result of UFC fights between any two athletes in the organization's roster, using comprehensive data analysis to deliver accurate fight predictions.

## This project is as an example of legacy code, for educational purposes only.

## How It Works

KoAi utilizes a flexible failover data provider by combining Cassandra database storage with live web scraping to achieve resilient data provisioning. Initially, we employ a UFC web scraper to clone the UFC database into a local Cassandra database and provide an API for fighter data retrieval. The system searches the Cassandra database first, using a primary key structure optimized for efficient queries by firstname:

### Primary Key Structure:
```
(first_name, gender, last_name, status, nick_name, age, division, fighting_style, img_url)
```

### Example Queries:
```
select * from athletes where first_name="john";
```
This query retrieves all fighters with the first name "John". To obtain a specific fighter, additional keys can be added starting with gender:

```
select * from athletes where first_name="john" and gender="male" and last_name="jones";
```

This structure facilitates both autocomplete functionality and comprehensive fighter data retrieval.

If a fighter cannot be located in the database, the failover search function automatically transitions to live web scraping to extract data directly from the UFC website, ensuring continuous data availability.

## Installation

The available method to test the KoAi application in your computer is by executing the configuration bash script, which installs all dependencies and builds the application:

```
chmod +x configure.sh
```

```
./configure.sh
```

## Basic Usage
The default start of KoAi app is by creating a new UFC database to achieve failover data providing, so the default start is recommended. See `main.cpp`

After executing `./configure.sh`, the script defines default Cassandra credentials with SimpleStrategy and a replication factor of 1, then clones the UFC database to your local Cassandra database. You can subsequently modify Cassandra configurations according to your requirements after it has been created.

Cassandra configuration adjustments can be made in `controllers/ufc_db_ctl_conf.hpp` , continue reading to learn how this can be done.

## Listening Port

If the `PORT` environment variable is not defined, the server will default to listening on port 18080, `https://0.0.0.0:<PORT>`.

### Creating a New Database Example:

```cpp
//main.cpp

//You don't need to create a database each time; you can run the code below separately
data_provider::DataProviderManager dataProvider;
database_ctl::UfcDataBaseControllerConf ufcCtlConfigurer;
database::Credentials cred;
cred.username = std::getenv(koai::exported::CASS_USERNAME);
cred.password = std::getenv(koai::exported::CASS_PASS);
if(cred.username==nullptr || cred.password==nullptr){
    auto logger = dataProvider.getLogger();
    logger->add("CASS_USERNAME | CASS_PASS environment variables not exported.\n");
    logger->log();
    logger->clear();
}else{
    std::string hosts ="127.0.0.1";
    dataProvider.configureDbController(ufcCtlConfigurer).
    setCredentials(std::move(hosts), std::move(cred));
}

//Create the database for all UFC athletes
dataProvider.createDatabase();

//Create the database for UFC athletes with name "john" only
dataProvider.createDatabase("john");

//Update the database
//dataProvider.updateDatabase(); 

//Update the database for athletes with name "john" only
//dataProvider.updateDatabase("john");

//Start the Crow server after creating the database
webserver::WebServer koAiServer;
koAiServer.listen();
```

## OS Compatibility

This project uses Apache Cassandra 5.0.3, which does not support Windows, so it is currently tested only on debian based Linux. If you require Windows compatibility, you may use an alternative Cassandra version at your discretion.


## SSL Certificate Configuration

Private Key File: Located at `/assets/key.pm`

Certificate File: Located at `/assets/c.pm`

## Requirements

This project requires Apache Cassandra 5.0.3, which depends on OpenJDK 17 and Python 3.11. Please ensure these dependencies are installed and configured as your system defaults, or simply execute `./configure.sh` to automatically establish all requirements.

This project also incorporates the following third-party libraries:

* cpp-driver for Cassandra: https://github.com/datastax/cpp-driver
  * Dependencies:
    * libuv (1.x)
    * OpenSSL
    * zlib

* Crow server for frontend and API integration with the KoAi application:
  https://github.com/CrowCpp/Crow
  
  * Dependencies:
    * asio
    * openssl
    * zlib
        

Required libraries can be installed via:
```
sudo apt-get update && sudo apt-get install -y cmake libuv1-dev libssl-dev zlib1g-dev nlohmann-json3-dev libxml2-dev libcurl4-openssl-dev libasio-dev
```

## Build

```
mkdir build && cd build
cmake ..
make
```

If you elect not to run `./configure.sh`, you must perform all setup steps manually, including downloading Apache Cassandra 5.0.3 and extracting it in the main directory.

## Prediction

I provide a default prediction strategy and striking-biased and wrestling-biased strategies that leverage UFC data and derived features to predict fight results between selected athletes. The Strategy design pattern for our prediction system make it extensible for custom algorithms to add in `predicter/strategies`.

## Architecture

<p align="center">
  <img src="https://github.com/biteval/koai/blob/main/diagram/koai-architect.png" alt="KoAI Architecture Diagram">
</p>

The primary KoAI modules include `/managers`, `/controllers`, `/utils/query_builder`, `/scraper`, and `/predicter`.

The following example demonstrates the interaction between these modules:

1.`/controllers/ufc_db_ctl_conf.hpp` defines a class `UfcDataBaseControllerConf`


2.`/utils/query_builder/query_builder_conf.hpp` defines a class `CassQueryBuilderConf`


3. `UfcDataBaseControllerConf` utilizes `CassQueryBuilderConf` object, which encapsulates essential Cassandra parameters such as clusterName, keySpace, etc:

```cpp
std::vector<std::string> primaryKey{FIRST_NAME_KEY, GENDER_KEY, LAST_NAME_KEY, STATUS_KEY, NICKNAME_KEY, AGE_KEY, DIVISION_KEY, FIGHTING_STYLE_KEY, IMAGE_URL_DB_KEY};
qry_builder_conf::CassQueryBuilderConf conf;
conf.clusterName="Ufc Cluster";
conf.keySpace="ufc";
conf.strategy="SimpleStrategy";
conf.tableName="athletes";
conf.replicationFactor="1";
conf.titles=std::move(titles);
conf.primaryKey=std::move(primaryKey);
```

4. The class `UfcDataBaseControllerConf` derived from base `ControllerConfigurer<UfcDataBaseControllerConf>`, this allows creating new `ControllerConfigurer` derived classes for diffrent configuratiuons.

 `UfcDataBaseControllerConf` is specifically designed to configure the `DataBaseController` class for the UFC Cassandra database by providing fundamental parameters for query generation. This can be customized to create alternative configurations.
 
5. `/utils/query_builder/cass_query_builder.hpp` defines a class `CassQueryBuilder`.

   `UfcDataBaseControllerConf` feeds the `CassQueryBuilder` with the `CassQueryBuilderConf` struct containing essential Cassandra parameters, enabling `CassQueryBuilder` to generate appropriate Cassandra queries, such as:
   ```cpp
   std::string getCreateTableQuery();
   std::string getCreateKeySpaceQuery();
   std::string getChangeClusterNameQuery();
   ```

   ```cpp
   database_utils::CassQueryBuilder qbuilder;
   //Move the conf struct to the Cass query builder obj.
   qbuilder.setConf(std::move(conf));

   //Other configurations
   qbuilder.withTitles().withTablePrimaryKey().setClusteringOrderBy(GENDER_KEY, database_utils::ORDER::ASC).
   setClusteringOrderBy(LAST_NAME_KEY, database_utils::ORDER::ASC).
   setClusteringOrderBy(STATUS_KEY, database_utils::ORDER::ASC).
   setClusteringOrderBy(NICKNAME_KEY, database_utils::ORDER::ASC).
   setClusteringOrderBy(AGE_KEY, database_utils::ORDER::ASC).
   setClusteringOrderBy(DIVISION_KEY, database_utils::ORDER::ASC).
   setClusteringOrderBy(FIGHTING_STYLE_KEY, database_utils::ORDER::ASC);
   //Move the configured qbuilder to dbController
   dbController.conf(std::move(qbuilder)); //dbController is a DataBaseController object
   ```

6. `/controllers/database_controller.hpp` defines the `DataBaseController` class which leverages `CassQueryBuilder` for query generation.

7. `/managers/data_provider_manager.hpp` defines a `DataProviderManager` class which employs `UfcDataBaseControllerConf` to configure the `DataBaseController`:
   ```cpp
   private:
   database_ctl::DataBaseController databaseController;
   public:
   template<typename Configurer>
   DataProviderManager &configureDbController(database_ctl::ControllerConfigurer<Configurer>&configurer){
       configurer.configure(databaseController); //configurer is UfcDataBaseControllerConf object
       return *this;
   }
   ```

8. `/scraper/ufc_scraper.hpp` defines a `UfcScraper` class.

9. `/managers/database_manager.hpp` defines a `UfcDatabaseManager` class.

   `DataProviderManager` contains private `UfcScraper` and `UfcDatabaseManager` members, injecting the `DataBaseController` and `UfcScraper` into the `UfcDatabaseManager`. The `UfcDatabaseManager` then utilizes these components to create or update the database:
   ```cpp
   void createDatabase(auto&dbCtl, auto&scraper);
   void updateDatabase(auto&dbCtl, auto&scraper);
   auto offlineSearch(auto&dbCtl, auto athleteName);
   ```

   ```cpp
   void database_manager::
   UfcDatabaseManager::createDatabase(auto&dbCtl, auto&scraper){
     dbCtl.changeClusterName();
     dbCtl.createKeySpace();
     dbCtl.createTable();
     dbCtl.setMode(database_ctl::MODE::ADD_NEW);
     scraper.setCurrDbMode(dbCtl.getCurrMode());  
     scraper.forEachRecord([&dbCtl](auto&&rec){  
       dbCtl.addRecord(std::move(rec));
     });
   }
   ```

The `UfcScraper` provides a `forEachRecord` function that accepts a lambda function for processing each scraped fighter record.

Please contact me if you require assistance or have any inquiries regarding this project.
