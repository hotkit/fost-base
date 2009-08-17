/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/datetime>
#include <fost/test>


FSL_TEST_SUITE( dates );


FSL_TEST_FUNCTION( constructors ) {
    fostlib::test::default_copy_constructable< fostlib::date >();
}
