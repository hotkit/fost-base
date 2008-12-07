/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema-test.hpp"
#include <fost/db.hpp>
#include <fost/string/utility.hpp>

#include <fost/exception/not_null.hpp>
#include <fost/exception/query_failure.hpp>


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
    //FSL_CHECK_EXCEPTION( master.create_database( L"base_database" ), exceptions::query_failure& );
}


FSL_TEST_FUNCTION( insert ) {
    string dbname( L"insert_databse" );
    {
        dbconnection master( L"master", L"master" );
        master.create_database( dbname );
    }
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
        FSL_CHECK_EXCEPTION( trans.commit(), exceptions::forwarded_exception& );
    }

    /*
        Create a new instance and save to the database
    */
    json first_init;
    jcursor()[ L"key" ]( first_init ) = 0;
    boost::shared_ptr< instance > first = simple.create( dbc, first_init );
    // Save, but don't commit
    {
        dbtransaction trans( dbc );
        first->save();
    }
    FSL_CHECK( !first->in_database() );
    // This time commit it
    {
        dbtransaction trans( dbc );
        first->save();
        trans.commit();
    }
    FSL_CHECK( first->in_database() );

    /*
        Create a second instance at the same key position
        As there is no object cache yet this will fail when it tries to commit
    */
    boost::shared_ptr< instance > first_alias = simple.create( dbc, first_init );
    {
        dbtransaction trans( dbc );
        first_alias->save();
        FSL_CHECK_EXCEPTION( trans.commit(), exceptions::forwarded_exception& );
    }
    FSL_CHECK( !first_alias->in_database() );
}


FSL_TEST_FUNCTION( recordset ) {
    dbconnection dbc( L"insert_databse" );

    meta_instance simple( L"simple" );
    simple.primary_key( L"key", L"integer" );

    FSL_CHECK( dbc.query( simple, json( 123 ) ).eof() );
    FSL_CHECK( !dbc.query( simple, json( 0 ) ).eof() );
}
