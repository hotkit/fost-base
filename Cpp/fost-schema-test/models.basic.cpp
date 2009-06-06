/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema-test.hpp"
#include <fost/db>


using namespace fostlib;


FSL_TEST_SUITE( basic_models );


class BasicModel : public model< BasicModel > {
public:
    BasicModel( const factory &f, dbconnection &dbc, const json &j )
    : superclass( f, dbc, j ) {
    }
    attribute< int64_t > pk;
};
template<>
const BasicModel::factory BasicModel::superclass::s_factory( L"BasicModel" );


class BasicSubModel : public model< BasicSubModel, BasicModel > {
public:
    BasicSubModel( const factory &f, dbconnection &dbc, const json &j )
    : superclass( f, dbc, j ) {
    }
    attribute< string > name;
};
template<>
const BasicSubModel::factory BasicSubModel::superclass::s_factory( L"BasicSubModel" );


class HostModel : public model< HostModel > {
public:
    HostModel( const factory &f, dbconnection &dbc, const json &j )
    : superclass( f, dbc, j ) {
    }

    class NestedModel : public model< NestedModel > {
    public:
        NestedModel( const factory &f, dbconnection &dbc, const json &j )
        : superclass( f, dbc, j ) {
        }
    };
};
template<>
const HostModel::factory HostModel::superclass::s_factory( L"HostModel" );
template<>
const HostModel::NestedModel::factory HostModel::NestedModel::superclass::s_factory( HostModel::s_factory, L"NestedModel" );

class SubHostModel : public model< SubHostModel, HostModel > {
public:
    SubHostModel( const factory &f, dbconnection &dbc, const json &j )
    : superclass( f, dbc, j ) {
    }
};
template<>
const SubHostModel::factory SubHostModel::superclass::s_factory( L"SubHostModel" );


FSL_TEST_FUNCTION( constructors_basic ) {
    dbconnection dbc( L"master", L"master" );
    boost::shared_ptr< BasicModel > instance = BasicModel::s_factory( dbc, json() );

    // We should always get the same meta_instance
    FSL_CHECK_EQ( &instance->meta().meta(), &instance->meta().meta() );
    // The name in the meta_instance must match the one we set
    FSL_CHECK_EQ( instance->meta().meta().name(), L"BasicModel" );
    // The instance's type is in the global namespace
    FSL_CHECK( instance->meta().meta().in_global() );
}


FSL_TEST_FUNCTION( constructors_subclass ) {
    dbconnection dbc( L"master", L"master" );
    boost::shared_ptr< BasicModel > i1 = BasicSubModel::s_factory( dbc, json() );
    boost::shared_ptr< BasicSubModel > i2 = BasicSubModel::s_factory( dbc, json() );

    FSL_CHECK( i1->meta().meta().in_global() );
    FSL_CHECK( i2->meta().meta().in_global() );
}


FSL_TEST_FUNCTION( constructors_nested ) {
    dbconnection dbc( L"master", L"master" );

    boost::shared_ptr< HostModel > i1 = HostModel::s_factory( dbc, json() );
    boost::shared_ptr< HostModel::NestedModel > i2 = HostModel::NestedModel::s_factory( dbc, json() );

    FSL_CHECK_EQ( i1->meta().meta().name(), L"HostModel" );
    FSL_CHECK( i1->meta().meta().in_global() );

    FSL_CHECK_EQ( i2->meta().meta().name(), L"NestedModel" );
    FSL_CHECK_EQ( i2->meta().meta().parent().name(), L"HostModel" );
    FSL_CHECK( !i2->meta().meta().in_global() );

    boost::shared_ptr< SubHostModel > i3 = SubHostModel::s_factory( dbc, json() );

    FSL_CHECK_EQ( i3->meta().meta().name(), L"SubHostModel" );
    FSL_CHECK( i3->meta().meta().in_global() );
}
