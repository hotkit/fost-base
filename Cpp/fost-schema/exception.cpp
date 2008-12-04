/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema.hpp"


#include <fost/exception/no_attribute.hpp>
fostlib::exceptions::no_attribute::no_attribute( const string &attribute ) throw ()
: exception( L"Missing attribute" ) {
    try {
        info() << L"Attribute: " << attribute << std::endl;
    } catch ( ... ) {
        absorbException();
    }
}
fostlib::wliteral const fostlib::exceptions::no_attribute::message() const throw () {
    return L"Attribute could not be found on the instance";
}

