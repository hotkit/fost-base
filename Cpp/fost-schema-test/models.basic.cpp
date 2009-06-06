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
FSL_TEST_FUNCTION( constructors_basic ) {
    BasicModel instance;
}


class BasicSubModel : public model< BasicSubModel, BasicModel > {
};
FSL_TEST_FUNCTION( constructors_subclass ) {
    BasicModel instance;
}
