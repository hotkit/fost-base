 /*
    Copyright 1999-2017, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/unicode>


using namespace fostlib;


FSL_TEST_SUITE( json_iterator );


FSL_TEST_FUNCTION( empty ) {
    json empty;

    fostlib::test::default_copy_constructable< json::const_iterator >();

    FSL_CHECK( json::const_iterator() == empty.begin() );
}


FSL_TEST_FUNCTION( key_object ) {
    json object = json::object_t();
    jcursor( "key1" ).insert( object, "value1" );
    jcursor( "key2" ).insert( object, "value2" );
    json::const_iterator p = object.begin();
    FSL_CHECK_EQ( *p, json( "value1" ) );
    FSL_CHECK_EQ(p->get<f5::u8view>().value(), "value1");
    FSL_CHECK_EQ( p.key(), json( "key1" ) );
    FSL_CHECK_NOTHROW( ++p );
    FSL_CHECK_EQ( *p, json( "value2" ) );
    FSL_CHECK_EQ( p.key(), json( "key2" ) );
    FSL_CHECK_NOTHROW( ++p );
}
