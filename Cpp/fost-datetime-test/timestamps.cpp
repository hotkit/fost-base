/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/datetime>
#include <fost/test>


FSL_TEST_SUITE( timestamps );


FSL_TEST_FUNCTION( constructors ) {
    fostlib::test::default_copy_constructable< fostlib::timestamp >();
}


FSL_TEST_FUNCTION( basic ) {
    fostlib::timestamp n(fostlib::timestamp::now());
    fostlib::string iso(fostlib::coerce< fostlib::string >(n));
    FSL_CHECK_EQ(iso.length(), 23);
    FSL_CHECK_EQ(iso[0], '2');
    FSL_CHECK_EQ(iso[1], '0');
    FSL_CHECK_EQ(iso[4], '-');

    FSL_CHECK_NOTHROW(fostlib::rfc1123_timestamp rfc1123(fostlib::coerce< fostlib::rfc1123_timestamp >(n)));
}
