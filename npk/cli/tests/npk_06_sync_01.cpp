#include "testutil.h"
#include <npk.h>

int npk_06_sync_01( int argc, char * argv [] )
{
    system( "../npk test.npk -create -add sample.txt@remain.txt sample.txt@update.txt sample.txt@notdelete.txt --f --k 1:2:3:4" );
    system( "mkdir sync_test_01" );
    system( "cp sample.txt sync_test_01/notadd.txt" );
    system( "cp sample.txt sync_test_01/remain.txt" );
    system( "cp sample2.txt sync_test_01/update.txt" );

    system( "../npk test.npk -sync sync_test_01 --k 1:2:3:4" );

    int teakey[4] = {1,2,3,4};
    NPK_PACKAGE pack = npk_package_open( "test.npk", teakey );

    CHECK( pack != NULL );

    NPK_ENTITY entity;

    entity = npk_package_get_entity( pack, "remain.txt" );
    CHECK( entity != NULL );

    entity = npk_package_get_entity( pack, "notadd.txt" );
    CHECK( entity == NULL );

    entity = npk_package_get_entity( pack, "notdelete.txt" );
    CHECK( entity != NULL );

    entity = npk_package_get_entity( pack, "update.txt" );
    CHECK( entity != NULL );

    NPK_SIZE size = npk_entity_get_size( entity );
    void* buf = malloc( size );

    CHECK( npk_entity_read( entity, buf ) );
    CHECK_EQUAL_STR_WITH_FILE( (const char*)buf, "sample2.txt" );

    free( buf );

    npk_package_close( pack );

    return 0;
}