/*
    Copyright 2009-2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/threading>


FSL_TEST_SUITE( workerpool );


FSL_TEST_FUNCTION( construction ) {
    fostlib::workerpool< void > pool1, pool2;
    fostlib::workerpool< int > pool3, pool4;
}


namespace {
    int check_exec_value = 0;
    void check_exec() {
        ++check_exec_value;
    }
}
FSL_TEST_FUNCTION( function ) {
    fostlib::workerpool< void > pool;
    int old_value = check_exec_value;
    pool( check_exec );
    pool();
    FSL_CHECK_EQ(check_exec_value, old_value + 1);
}

namespace {
    int quadrupal( int d ) {
        return d * 4;
    }
}
FSL_TEST_FUNCTION( function_int ) {
    fostlib::workerpool< int > pool;
    boost::function< int ( void ) > quad = boost::lambda::bind( quadrupal, 2000 );

    pool( quad );
    FSL_CHECK_EQ( pool(), 8000 );

    /*fostlib::future< int > q1 = pool.f<int>(quad), q2 = pool.f<int>(quad);
    FSL_CHECK_EQ( q1(), 8000 );
    FSL_CHECK_EQ( q2(), 8000 );*/
}
