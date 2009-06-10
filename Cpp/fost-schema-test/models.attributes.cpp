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
    attribute< int64_t, a_primary > pk;
    attribute< string > display_name;
};
FSL_MODEL( SimpleModel )( L"SimpleModel" );
template<> template<>
const SimpleModel::attribute< int64_t, SimpleModel::a_primary >::attribute_binding SimpleModel::attribute< int64_t, SimpleModel::a_primary >::binding( L"pk" );
template<> template<>
const SimpleModel::attribute< string >::attribute_binding SimpleModel::attribute< string >::binding( L"display_name" );

class SubModel : public model< SubModel, SimpleModel > {
public:
    SubModel( const factory &f, dbconnection &dbc, const json &j )
    : superclass( f, dbc, j ) {
    }
    attribute< string > name;
};
FSL_MODEL( SubModel )( L"SubModel" );


FSL_TEST_FUNCTION( base_attribute ) {
    FSL_CHECK_EQ( ( SimpleModel::attribute< int64_t, SimpleModel::a_primary >::binding.name() ), L"pk" );
    FSL_CHECK_NOTHROW( SimpleModel::_meta()[ L"pk" ] );

    FSL_CHECK_EQ( SimpleModel::attribute< string >::binding.name(), L"display_name" );
    FSL_CHECK_NOTHROW( SimpleModel::_meta()[ L"display_name" ] );
}
