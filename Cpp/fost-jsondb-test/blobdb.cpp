/*
    Copyright 2007-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-jsondb-test.hpp"
#include <fost/jsondb>

#include <fost/exception/json_error.hpp>
#include <fost/exception/not_null.hpp>
#include <fost/exception/out_of_range.hpp>


using namespace fostlib;


FSL_TEST_SUITE( blob );


FSL_TEST_FUNCTION( construct ) {
    FSL_CHECK_NOTHROW( jsondb database );
    FSL_CHECK_NOTHROW( jsondb database; jsondb::local loc( database ) );
}


FSL_TEST_FUNCTION( insert ) {
    jsondb database;
    jsondb::local loc1( database ), loc2( database );

    /*
        First of all just put something into the database and check isolation
    */
    FSL_CHECK_NOTHROW( loc1.insert( jcursor(), json( true ) ).commit() );
    // loc2 won't see it yet
    FSL_CHECK( loc2[ jcursor() ].isnull() );
    // loc1 does
    FSL_CHECK_EQ( loc1[ jcursor() ], json( true ) );
    // A new local will also see it
    FSL_CHECK_EQ( jsondb::local( database )[ jcursor() ], json( true ) );

    /*
        Do some error checking
    */
    // We can't add to loc1 as it already has something at this position
    FSL_CHECK_EXCEPTION( loc1.insert( jcursor(), json( 10 ) ), exceptions::not_null& );
    // It looks like it works on loc2
    FSL_CHECK_NOTHROW( loc2.insert( jcursor(), json( 10 ) ) );
    // But it throws when we commit
    FSL_CHECK_EXCEPTION( loc2.commit(), exceptions::forwarded_exception& );
}


FSL_TEST_FUNCTION( update ) {
    jsondb database;
    jsondb::local loc1( database );

    /*
        We need something in the object to start with
    */
    FSL_CHECK_NOTHROW( loc1.insert( jcursor()[ L"hello" ], json( L"world" ) ).commit() );
    // Change the value to something new
    FSL_CHECK_NOTHROW( loc1.update( jcursor()[ L"hello" ], json( L"goodbye" ) ).commit() );
    FSL_CHECK_EQ( loc1[ L"hello" ], json( L"goodbye" ) );

    // Try to update a non-existent location
    FSL_CHECK_EXCEPTION( loc1.update( jcursor()[ L"empty" ], json( L"won't work" ) ), exceptions::null& );
}


FSL_TEST_FUNCTION( remove ) {
    jsondb database;
    jsondb::local loc1( database ), loc2( database );

    /*
        We need something in the object to start with
    */
    FSL_CHECK_NOTHROW( loc1
        .insert( jcursor()[ L"hello" ], json( L"nightclub" ) )
        .insert( jcursor()[ L"goodbye" ], json( L"country" ) )
        .commit()
    );
    FSL_CHECK_EQ( loc1[ L"hello" ], json( L"nightclub" ) );
    FSL_CHECK_EQ( loc1[ L"goodbye" ], json( L"country" ) );

    // We must have at least one level of path in the jcursor
    FSL_CHECK_EXCEPTION( loc1.remove( jcursor() ), exceptions::out_of_range< std::size_t >& );

    // Deleting from a non-existent key will fail straight away
    FSL_CHECK_EXCEPTION( loc1.remove( jcursor()[ L"not a key" ] ), exceptions::json_error& );

    // Let's delete and update
    FSL_CHECK_NOTHROW( loc1
        .remove( jcursor()[ L"goodbye" ] )
        .update( jcursor()[ L"hello" ], L"world" )
        .commit()
    );
    FSL_CHECK_EQ( loc1[ L"hello" ], json( L"world" ) );
    FSL_CHECK( !loc1.has_key( L"goodbye"  ) );
}
