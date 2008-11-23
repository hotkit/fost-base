/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema.hpp"
#include <fost/schema.hpp>


using namespace fostlib;


fostlib::meta_instance::meta_instance( const string &n )
: name( n ) {
}

string fostlib::meta_instance::table( const instance_base & ) const {
    return name();
}
