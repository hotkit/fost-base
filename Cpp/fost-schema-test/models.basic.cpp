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
    : model< BasicModel >( f, dbc, j ) {
    }
    attribute< int64_t > pk;
};
template<>
const BasicModel::factory fostlib::model< BasicModel >::s_factory =
        BasicModel::factory( L"BasicModel" );


class BasicSubModel : public model< BasicSubModel, BasicModel > {
public:
    BasicSubModel( const factory &f, dbconnection &dbc, const json &j )
    : model< BasicSubModel, BasicModel >( f, dbc, j ) {
    }
    attribute< string > name;
};
template<>
const BasicSubModel::factory fostlib::model< BasicSubModel, BasicModel >::s_factory =
        BasicSubModel::factory( L"BasicSubModel" );


class HostModel : public model< HostModel > {
public:
    HostModel( const factory &f, dbconnection &dbc, const json &j )
    : model< HostModel >( f, dbc, j ) {
    }

    class NestedModel : public model< NestedModel > {
    public:
        NestedModel( const factory &f, dbconnection &dbc, const json &j )
        : model< NestedModel >( f, dbc, j ) {
        }


    };
};
template<>
const HostModel::factory fostlib::model< HostModel >::s_factory =
        HostModel::factory( L"HostModel" );
template<>
const HostModel::NestedModel::factory fostlib::model< HostModel::NestedModel >::s_factory =
        HostModel::NestedModel::factory( HostModel::s_factory, L"NestedModel" );


FSL_TEST_FUNCTION( constructors_basic ) {
    dbconnection dbc( L"master", L"master" );
    boost::shared_ptr< BasicModel > instance = BasicModel::s_factory( dbc, json() );

    // We should always get the same meta_instance
    FSL_CHECK_EQ( &instance->meta().meta(), &instance->meta().meta() );
    // The name in the meta_instance must match the one we set
    FSL_CHECK_EQ( instance->meta().meta().name(), L"BasicModel" );

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
}
