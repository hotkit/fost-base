/*
    Copyright 2007-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-jsondb-test.hpp"
#include <fost/jsondb>

#include <fost/exception/not_null.hpp>


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
