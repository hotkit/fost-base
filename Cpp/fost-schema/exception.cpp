/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema.hpp"
#include <fost/db.hpp>


fostlib::exceptions::data_driver::data_driver( const string&m, const string &d ) throw ()
: exception( m ) {
    m_info << L"Driver: " << d << std::endl;
}
fostlib::exceptions::data_driver::data_driver( const string &m, const string &d1, const string &d2 ) throw ()
: exception( m ) {
    m_info << L"Driver 1: " << d1 << L"\nDriver 2: " << d2 << std::endl;
}
fostlib::wliteral const fostlib::exceptions::data_driver::message() const throw () {
    return L"Problem with database driver.";
}


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


#include <fost/exception/query_failure.hpp>
fostlib::exceptions::query_failure::query_failure( const string &m, const fostlib::meta_instance &i ) throw ()
: exception( m ) {
    try {
        info() << L"Meta instance: " << i.fq_name() << std::endl;
    } catch ( ... ) {
        absorbException();
    }
}
fostlib::wliteral const fostlib::exceptions::query_failure::message() const throw () {
    return L"Problem executing query";
}

