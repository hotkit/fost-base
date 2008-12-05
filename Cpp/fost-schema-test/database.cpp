/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema-test.hpp"
#include <fost/db.hpp>
#include <fost/exception/null.hpp>


using namespace fostlib;


FSL_TEST_SUITE( simple_schema );


FSL_TEST_FUNCTION( simple ) {
    meta_instance simple( L"simple" );
    simple
        .primary_key( L"id", L"integer" )
        .field( L"name", L"varchar", true, 10 );

    dbconnection dbc( L"" );
}

