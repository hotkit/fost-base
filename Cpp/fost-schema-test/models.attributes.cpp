/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema-test.hpp"
#include <fost/db>


using namespace fostlib;


FSL_TEST_SUITE( basic_attributes );


class SimpleModel : public model< SimpleModel > {
public:
    SimpleModel( const factory &f, dbconnection &dbc, const json &j )
    : superclass( f, dbc, j ) {
    }
    attribute< int64_t > pk;
};
template<>
const SimpleModel::factory SimpleModel::superclass::s_factory( L"SimpleModel" );
template<> template<>
const SimpleModel::attribute< int64_t >::attribute_binding SimpleModel::attribute< int64_t >::binding( L"pk" );

class SubModel : public model< SubModel, SimpleModel > {
public:
    SubModel( const factory &f, dbconnection &dbc, const json &j )
    : superclass( f, dbc, j ) {
    }
    attribute< string > name;
};
template<>
const SubModel::factory SubModel::superclass::s_factory( L"SubModel" );


FSL_TEST_FUNCTION( base_attribute ) {
    FSL_CHECK_EQ( SimpleModel::attribute< int64_t >::binding.name(), L"pk" );
    FSL_CHECK_NOTHROW( SimpleModel::_meta()[ L"pk" ] );
}
