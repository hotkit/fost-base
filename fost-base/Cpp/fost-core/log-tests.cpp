/*
    Copyright 2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/log>


FSL_TEST_SUITE( log );


FSL_TEST_FUNCTION( log ) {
    using namespace fostlib::logging;
    log(debug, "Debug level");
    log(info, "Info level");
}


FSL_TEST_FUNCTION( direct ) {
    using namespace fostlib::logging;
    debug("Debug level");
    info("Debug level");
}
