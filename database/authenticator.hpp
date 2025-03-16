#ifndef __KOAI_CASS_AUTHENTICATOR__WRAPPER__
#define __KOAI_CASS_AUTHENTICATOR__WRAPPER__

#include <cassandra.h>
#include <cstring>

namespace database{
    struct Credentials{
        const char*username;
        const char*password;
        Credentials();
        Credentials(const Credentials&)=delete;
        Credentials&operator=(const Credentials&)=delete;
        Credentials(Credentials&&)noexcept = default;
        Credentials&operator=(Credentials&&)noexcept=default;
        bool isProvided()const;
    };
}

namespace database{
    void on_auth_initial(CassAuthenticator*, void*);
    void on_auth_challenge(CassAuthenticator*, void*, const char*, size_t );
    void on_auth_success(CassAuthenticator* , void* , const char* , size_t );
    void on_auth_cleanup(CassAuthenticator* , void* );
}

namespace database{
    class Authenticator{
        public:
        Authenticator();
        Authenticator(const Authenticator&)=delete;
        Authenticator operator=(const Authenticator&)=delete;
        void authenticate(CassCluster*cluster,Credentials&cred);
        ~Authenticator();
    };
}

#endif // ! __KOAI_CASS_AUTHENTICATOR__WRAPPER__ INCLDUED
