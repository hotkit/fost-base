/*
    Copyright 2012, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/datetime>
#include <fost/test>


using namespace fostlib;


FSL_TEST_SUITE( timestamps_rfc1123 );


FSL_TEST_FUNCTION( roundtrip ) {
    timestamp ts(2012, 8, 12, 15, 33, 24);
    rfc1123_timestamp s1("Sun, 12 Aug 2012 15:33:24 +0000");
    FSL_CHECK_EQ(coerce<rfc1123_timestamp>(ts), s1);
    FSL_CHECK_EQ(coerce<timestamp>(s1), ts);
}


// FSL_TEST_FUNCTION( without_dayname ) {
//     timestamp ts(2012, 8, 12, 15, 33, 24);
//     rfc1123_timestamp s1("12 Aug 2012 15:33:24 +0000");
//     FSL_CHECK_EQ(coerce<timestamp>(s1), ts);
// }

