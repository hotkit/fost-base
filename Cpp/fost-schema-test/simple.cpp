/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema-test.hpp"
#include <fost/schema.hpp>


using namespace fostlib;


FSL_TEST_SUITE( simple_schema );


FSL_TEST_FUNCTION( dynamic ) {
    meta_instance simple( L"simple" );
    simple
        .primary_key( L"id", L"int" )
        .field( L"name", L"varchar", 100 )
        .field( L"counter", L"decimal", 15, 2 );
}


namespace {
    class simple : public model< simple > {
    };
    class subsimple : public model< subsimple, simple > {
    };
}
FSL_TEST_FUNCTION( static ) {
}
