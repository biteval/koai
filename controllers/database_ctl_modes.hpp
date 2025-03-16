#ifndef  __KOAI_DATABASE_CTL_MODES__
#define  __KOAI_DATABASE_CTL_MODES__

namespace database_ctl{
    /*
    ADD_NEW : add new record to the database
    RETRIEVE: retirve a record from local database
    COMMING_FROM_UPDATE: updating a record from UPDATE mode to ADD_NEW mode
    LIVE: reading records live remote server
    */
    enum MODE{ADD_NEW,UPDATE, RETRIEVE ,COMMING_FROM_UPDATE,LIVE};
}
#endif // ! __KOAI_DATABASE_CTL_MODES__ INCLUDED