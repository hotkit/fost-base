/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"

#include <fost/thread.hpp>


namespace {
    struct executor {
        fostlib::counter &counter;
        executor( fostlib::counter &c )
        : counter( c ) {
            ++counter;
        }
        int value() {
            return counter.value();
        }
    };
    executor *factory( fostlib::counter &c ) {
        return new executor( c );
    }
}


FSL_TEST_SUITE( thread );


FSL_TEST_FUNCTION( in_proc_constructor ) {
    fostlib::counter count;
    fostlib::in_process< executor > ipc2( new executor( count ) );
    fostlib::in_process< executor > ipc1( boost::lambda::bind( factory, boost::ref( count ) ) );
    FSL_CHECK_EQ( count.value(), 2 );
}


FSL_TEST_FUNCTION( in_proc_futures ) {
    fostlib::counter count;
    fostlib::in_process< executor > ipc( new executor( count ) );
    FSL_CHECK_EQ( count.value(), 1 );
    FSL_CHECK_EQ( ipc.synchronous< int >( boost::lambda::bind( &executor::value, boost::lambda::_1 ) ), 1 );
    FSL_CHECK_EQ( ipc.asynchronous< int >( boost::lambda::bind( &executor::value, boost::lambda::_1 ) )(), 1 );
}


FSL_TEST_FUNCTION( threadsafe_store_remove ) {
    fostlib::threadsafe_store< int, int > lib;
    lib.add( 2, 2 );
    lib.add( 2, 3 );
    FSL_CHECK_EQ( lib.find( 2 ).size(), 2 );
    FSL_CHECK_EQ( *lib.find( 2 ).begin(), 2 );
    FSL_CHECK( lib.remove( 2, 2 ) );
    FSL_CHECK_EQ( lib.find( 2 ).size(), 1 );
    FSL_CHECK_EQ( *lib.find( 2 ).begin(), 3 );
    FSL_CHECK( lib.remove( 2 ) );
    FSL_CHECK_EQ( lib.find( 2 ).size(), 0 );
    FSL_CHECK( !lib.remove( 2 ) );
    FSL_CHECK( !lib.remove( 2, 3 ) );
    lib.add( 2, 2 );
    FSL_CHECK( lib.remove( 2, 2 ) );
}
