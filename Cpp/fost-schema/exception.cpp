/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema.hpp"
#include <fost/db.hpp>


using namespace fostlib;


fostlib::exceptions::data_driver::data_driver( const string&m, const string &d ) throw ()
: exception( m ) {
    m_info << L"Driver: " << d << L"\n";
}
fostlib::exceptions::data_driver::data_driver( const string &m, const string &d1, const string &d2 ) throw ()
: exception( m ) {
    m_info << L"Driver 1: " << d1 << L"\nDriver 2: " << d2 << L"\n";
}
wliteral const fostlib::exceptions::data_driver::message() const throw () {
    return L"Problem with database driver.";
}


fostlib::exceptions::transaction_fault::transaction_fault( const string &error ) throw ()
: exception( error ) {
}
wliteral const fostlib::exceptions::transaction_fault::message() const throw () {
    return L"Fault in transaction handling.";
}
