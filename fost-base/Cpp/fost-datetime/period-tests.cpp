/*
    Copyright 2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/datetime>
#include <fost/test>


FSL_TEST_SUITE( period );


FSL_TEST_FUNCTION( constructors ) {
    fostlib::test::default_copy_constructable< fostlib::period< void > >();
    fostlib::test::default_copy_constructable< fostlib::period< int > >();
    fostlib::test::default_copy_constructable< fostlib::period< fostlib::string > >();
    fostlib::period< void > p1(
        fostlib::timestamp(2010, 1, 1), fostlib::timestamp::now()
    );
}


FSL_TEST_FUNCTION( basic ) {
}


FSL_TEST_FUNCTION( json ) {
}
