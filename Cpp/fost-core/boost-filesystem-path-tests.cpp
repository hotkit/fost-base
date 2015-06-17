/*
    Copyright 2009-2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/unicode>


FSL_TEST_SUITE( boost_filesystem_path );


FSL_TEST_FUNCTION( coercion_string ) {
    boost::filesystem::path p( L"somefilepath" );
    FSL_CHECK_EQ(fostlib::coerce< fostlib::string >(p), L"somefilepath");
}


FSL_TEST_FUNCTION( coercion_json ) {
    boost::filesystem::path p( L"somefilepath" );
    fostlib::json j( L"somefilepath" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::json >(p), j );
    FSL_CHECK_EQ( fostlib::coerce< boost::filesystem::path >(j), p );
}
