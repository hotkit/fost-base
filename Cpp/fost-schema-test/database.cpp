/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema-test.hpp"
#include <fost/db.hpp>
#include <fost/string/utility.hpp>

#include <fost/thread.hpp>

#include <fost/exception/not_null.hpp>
#include <fost/exception/out_of_range.hpp>
#include <fost/exception/query_failure.hpp>
#include <fost/exception/unexpected_eof.hpp>


using namespace fostlib;


FSL_TEST_SUITE( database );


FSL_TEST_FUNCTION( checks ) {
    FSL_CHECK_EXCEPTION( dbconnection( L"master", L"different" ), exceptions::data_driver& );

    FSL_CHECK_EXCEPTION(
        dbconnection( L"master" ).create_database( L"base_database" ),
        exceptions::transaction_fault&
    );

    dbconnection master( L"master", L"master" );
    FSL_CHECK_NOTHROW( master.create_database( L"base_database" ) );
    //FSL_CHECK_EXCEPTION( master.create_database( L"base_database" ), exceptions::query_failure& );
}


// This test is also used by jsondb-file.cpp
#ifdef WIN32
__declspec( dllexport )
#endif
void do_insert_test( dbconnection &dbc ) {
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
FSL_TEST_FUNCTION( insert ) {
    string dbname( L"insert_database" );
    {
        dbconnection master( L"master", L"master" );
        master.create_database( dbname );
    }
    dbconnection dbc( dbname, dbname );
    do_insert_test( dbc );
}


FSL_TEST_FUNCTION( recordset ) {
    dbconnection dbc( L"insert_database" );

    meta_instance simple( L"simple" );
    simple.primary_key( L"key", L"integer" );

    FSL_CHECK( dbc.query( simple, json( 123 ) ).eof() );
    FSL_CHECK( !dbc.query( simple, json( 0 ) ).eof() );

    recordset rs( dbc.query( simple ) );
    FSL_CHECK_EQ( rs.fields(), 1 );
    FSL_CHECK_EQ( rs.field( L"key" ), json( 0 ) );
    FSL_CHECK_EQ( rs.field( 0 ), json( 0 ) );
    FSL_CHECK_EXCEPTION( rs.field( 1 ), exceptions::out_of_range< std::size_t >& );
    rs.moveNext();
    FSL_CHECK( rs.eof() );
    FSL_CHECK_EXCEPTION( rs.moveNext(), exceptions::unexpected_eof& );
}


FSL_TEST_FUNCTION( transactions ) {
    dbconnection dbc1( L"insert_database" ), dbc2( L"insert_database" );

    meta_instance simple( L"simple" );
    simple.primary_key( L"key", L"integer" );

    FSL_CHECK( !dbc1.query( simple, json( 0 ) ).eof() );
    FSL_CHECK( !dbc2.query( simple, json( 0 ) ).eof() );
    FSL_CHECK( dbc1.query( simple, json( 1 ) ).eof() );
    FSL_CHECK( dbc2.query( simple, json( 1 ) ).eof() );

    /*
        We'll creat and save an object to dbc1
    */
    json second_init, third_init;
    jcursor()[ L"key" ]( second_init ) = 1;
    boost::shared_ptr< instance > second = simple.create( dbc1, second_init );
    {
        dbtransaction trans( dbc1 );
        second->save();
        trans.commit();
    }

    // object 1 should only be visible on dbc1 due to the read transaction dbc2 is still in
    try {
        FSL_CHECK( !dbc1.query( simple, json( 0 ) ).eof() );
        FSL_CHECK( !dbc2.query( simple, json( 0 ) ).eof() );
        FSL_CHECK( !dbc1.query( simple, json( 1 ) ).eof() );
        FSL_CHECK( dbc2.query( simple, json( 1 ) ).eof() );
    } catch ( exceptions::exception &e ) {
        e.info() << L"dbc1 simple: " << json::unparse( dbc1.query( simple ).to_json(), true )
            << L"dbc2 simple: " << json::unparse( dbc2.query( simple ).to_json(), true );
        throw;
    }

    /*
        Now create another new object on dbc2. dbc1 won't see it because of its read transaction
        started after the last transaciton's commit.
    */
    FSL_CHECK_NOTHROW( jcursor()[ L"key" ]( third_init ) = 2 );
    boost::shared_ptr< instance > third = simple.create( dbc2, third_init );
    {
        dbtransaction trans( dbc2 );
        FSL_CHECK_NOTHROW( third->save() );
        FSL_CHECK_NOTHROW( trans.commit() );
    }
    // dbc2 will now see 'first' due to its new read transaction
    FSL_CHECK( !dbc1.query( simple, json( 0 ) ).eof() );
    FSL_CHECK( !dbc2.query( simple, json( 0 ) ).eof() );
    FSL_CHECK( !dbc1.query( simple, json( 1 ) ).eof() );
    FSL_CHECK( !dbc2.query( simple, json( 1 ) ).eof() );
    FSL_CHECK( dbc1.query( simple, json( 2 ) ).eof() );
    FSL_CHECK( !dbc2.query( simple, json( 2 ) ).eof() );

    // The new dbc3 will see everything as it is created after the commits
    dbconnection dbc3( L"insert_database" );
    FSL_CHECK( !dbc3.query( simple, json( 0 ) ).eof() );
    FSL_CHECK( !dbc3.query( simple, json( 1 ) ).eof() );
    FSL_CHECK( !dbc3.query( simple, json( 2 ) ).eof() );

    /*
        To test atomicity we want to check various transaction problems
    */
    json fourth_init, fifth_init;
    FSL_CHECK_NOTHROW( jcursor()[ L"key" ]( fourth_init ) = 3 );
    boost::shared_ptr< instance > fourth = simple.create( dbc2, fourth_init );

    // This should trivially work as the transaction doesn't even try to update anything
    // until the commit
    {
        dbtransaction trans( dbc2 );
        FSL_CHECK_NOTHROW( fourth->save() );
    }
    FSL_CHECK( dbc2.query( simple, json( 3 ) ).eof() );

    // Now try one where we get an error during the commit part
    boost::shared_ptr< instance > fifth = simple.create( dbc2, fourth_init );
    {
        dbtransaction trans( dbc2 );
        FSL_CHECK_NOTHROW( fourth->save() );
        FSL_CHECK_NOTHROW( fifth->save() );
        FSL_CHECK_EXCEPTION( trans.commit(), exceptions::forwarded_exception& );
    }
    FSL_CHECK( dbc2.query( simple, json( 3 ) ).eof() );
}
