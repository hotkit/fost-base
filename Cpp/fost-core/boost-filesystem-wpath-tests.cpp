/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/unicode>


FSL_TEST_SUITE( boost_filesystem_wpath );


FSL_TEST_FUNCTION( coercion_string ) {
    boost::filesystem::wpath p( L"somefilepath" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >(p), L"somefilepath" );
}


FSL_TEST_FUNCTION( coercion_json ) {
    boost::filesystem::wpath p( L"somefilepath" );
    fostlib::json j( L"somefilepath" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::json >(p), j );
    FSL_CHECK_EQ( fostlib::coerce< boost::filesystem::wpath >(j), p );
}
