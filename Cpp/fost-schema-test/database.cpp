/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema-test.hpp"
#include <fost/db.hpp>
#include <fost/string/utility.hpp>


using namespace fostlib;


FSL_TEST_SUITE( database );


FSL_TEST_FUNCTION( simple ) {
    FSL_CHECK_EXCEPTION( dbconnection( L"master", L"different" ), exceptions::data_driver& );
    dbconnection master( L"master", L"master" );
    string dbname( guid() );
    master.create_database( dbname );
    dbconnection dbc( dbname, dbname );
}

