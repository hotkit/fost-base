/*
    Copyright 2009-2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/threading>

#include <chrono>
#include <thread>


FSL_TEST_SUITE( workerpool );


FSL_TEST_FUNCTION( construction ) {
    fostlib::workerpool pool1, pool2;
}


namespace {
    int quadrupal(int d) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return d * 4;
    }
}
FSL_TEST_FUNCTION( simple_function ) {
    fostlib::workerpool pool;
    std::function< int ( void ) > quad = []() { return quadrupal(2000); };
    fostlib::future< int > q = pool.f<int>( quad );
    FSL_CHECK_EQ( q(), 8000 );
    FSL_CHECK_EQ( pool.available(), 1u );
    FSL_CHECK_EQ( pool.peak_used(), 1u );

    fostlib::future< int > q1 = pool.f<int>(quad), q2 = pool.f<int>(quad);
    FSL_CHECK_EQ( q1(), 8000 );
    FSL_CHECK_EQ( q2(), 8000 );
    FSL_CHECK_EQ( pool.available(), 2u );
    FSL_CHECK_EQ( pool.peak_used(), 2u );
}

