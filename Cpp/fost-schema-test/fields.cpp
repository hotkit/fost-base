/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema-test.hpp"
#include <fost/schema.hpp>
#include <fost/exception/out_of_range.hpp>


using namespace fostlib;


FSL_TEST_SUITE( fields );


FSL_TEST_FUNCTION( define ) {
    const field_wrapper< int64_t > i( L"int64" );
}


FSL_TEST_FUNCTION( registry ) {
    FSL_CHECK_EQ( field_base::fetch( L"integer" ).type_name(), L"integer" );
    FSL_CHECK_EXCEPTION( field_base::fetch( L"zzzz" ), exceptions::out_of_range< std::size_t >& );
    {
        const field_wrapper< int64_t > i( L"integer" );
        FSL_CHECK_EXCEPTION( field_base::fetch( L"integer" ), exceptions::out_of_range< std::size_t >& );
    }
    FSL_CHECK_EQ( field_base::fetch( L"integer" ).type_name(), L"integer" );
}
