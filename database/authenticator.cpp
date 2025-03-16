#include "authenticator.hpp"


database::Credentials::Credentials(){
    username=nullptr;
    password=nullptr;
}

bool database::Credentials::isProvided()const{
  return username!=nullptr && password!=nullptr;
}

database::Authenticator::Authenticator(){} 

void database::on_auth_initial(CassAuthenticator* auth, void* data) {
/*
  * This callback is used to initiate a request to begin an authentication
  * exchange. Required resources can be acquired and initialized here.
  *
  * Resources required for this specific exchange can be stored in the
  * auth->data field and will be available in the subsequent challenge
  * and success phases of the exchange. The cleanup callback should be used to
  * free these resources.
  */

/*
  * The data parameter contains the credentials passed in when the
  * authentication callbacks were set and is available to all
  * authentication exchanges.
  */
  const Credentials* credentials = (const Credentials*)data;

  size_t username_size = strlen(credentials->username);
  size_t password_size = strlen(credentials->password);
  size_t size = username_size + password_size + 2;

  char* response = cass_authenticator_response(auth, size);

  /* Credentials are prefixed with '\0' */
  response[0] = '\0';
  memcpy(response + 1, credentials->username, username_size);

  response[username_size + 1] = '\0';
  memcpy(response + username_size + 2, credentials->password, password_size);
}

void database::on_auth_challenge(CassAuthenticator* , void* , const char* , size_t ) {
/*
  * Not used for plain text authentication, but this is to be used
  * for handling an authentication challenge initiated by the server.
  */
}

void database::on_auth_success(CassAuthenticator* , void* , const char* , size_t ) {
/*
  * Not used for plain text authentication, but this is to be used
  * for handling the success phase of an exchange.
  */
}

void database::on_auth_cleanup(CassAuthenticator*, void*) {
/*
  * No resources cleanup is necessary for plain text authentication, but
  * this is used to cleanup resources acquired during the authentication
  * exchange.
  */
}


void database::Authenticator::authenticate(CassCluster*cluster,Credentials&cred){
/* Setup authentication callbacks and credentials */
CassAuthenticatorCallbacks auth_callbacks = { on_auth_initial, on_auth_challenge, on_auth_success,
on_auth_cleanup };
/* Set custom authentication callbacks and credentials */
cass_cluster_set_authenticator_callbacks(cluster, &auth_callbacks, NULL, &cred);
}

database::Authenticator::~Authenticator(){}


