/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema-test.hpp"
#include <fost/db.hpp>
#include <fost/string/utility.hpp>


using namespace fostlib;


FSL_TEST_SUITE( database );


FSL_TEST_FUNCTION( checks ) {
    FSL_CHECK_EXCEPTION( dbconnection( L"master", L"different" ), exceptions::data_driver& );

    FSL_CHECK( dbconnection( L"master" ).read_only() );
    FSL_CHECK_EXCEPTION(
        dbconnection( L"master" ).create_database( L"base_database" ),
        exceptions::transaction_fault&
    );

    dbconnection master( L"master", L"master" );
    master.create_database( L"base_database" );
    master.create_database( L"base_database" ); // Should give an error
}


FSL_TEST_FUNCTION( insert ) {
    dbconnection master( L"master", L"master" );
    string dbname( guid() );
    master.create_database( dbname );
    dbconnection dbc( dbname, dbname );

    meta_instance simple( L"simple" );
    simple.primary_key( L"key", L"integer" );

    {
        dbtransaction trans( dbc );
        trans.create_table( simple );
        trans.commit();
    }
    {
        dbtransaction trans( dbc );
        trans.create_table( simple );
        FSL_CHECK_EXCEPTION( trans.commit(), fostlib::exceptions::forwarded_exception& );
    }

    json first_init;
    jcursor()[ L"key" ]( first_init ) = 0;
    boost::shared_ptr< instance > first = simple.create( dbc, first_init );
    {
        dbtransaction trans( dbc );
        first->save();
        trans.commit();
    }
    FSL_CHECK( first->in_database() );
}
