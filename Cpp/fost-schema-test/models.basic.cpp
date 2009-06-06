/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema-test.hpp"
#include <fost/schema.hpp>


using namespace fostlib;


FSL_TEST_SUITE( basic_models );


class BasicModel : public model< BasicModel > {
public:
    attribute< int64_t > pk;
};
template<> const BasicModel::factory fostlib::model< BasicModel >::s_factory = BasicModel::factory();
FSL_TEST_FUNCTION( constructors_basic ) {
    boost::shared_ptr< BasicModel > instance = BasicModel::s_factory.create();
}


class BasicSubModel : public model< BasicSubModel, BasicModel > {
};
template<> const BasicSubModel::factory fostlib::model< BasicSubModel, BasicModel >::s_factory = BasicSubModel::factory();
FSL_TEST_FUNCTION( constructors_subclass ) {
    boost::shared_ptr< BasicModel > i1 = BasicSubModel::s_factory.create();
    boost::shared_ptr< BasicSubModel > i2 = BasicSubModel::s_factory.create();
}
