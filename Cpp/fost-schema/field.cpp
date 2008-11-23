/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema.hpp"
#include <fost/schema.hpp>


using namespace fostlib;


library< field_base > &fostlib::field_base::registry() {
    static library< field_base > lib;
    return lib;
}
