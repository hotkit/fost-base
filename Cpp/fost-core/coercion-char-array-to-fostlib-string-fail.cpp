/*
    Copyright 2008-2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"

void fail() {
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( "3.141f" ), L"3.141f" );
}
