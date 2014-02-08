/*
    Copyright 2010-2012, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/parse/parse.hpp>


using namespace fostlib;


boost::recursive_mutex &fostlib::detail::g_parser_mutex() {
    static boost::recursive_mutex m;
    return m;
}

