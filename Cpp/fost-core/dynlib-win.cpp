/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


using namespace fostlib;


struct fostlib::dynlib::impl {
};


fostlib::dynlib::dynlib( const string &pathname )
: m_lib( NULL ) {
    if ( ::LoadLibrary( pathname.c_str() ) == NULL )
        throw fostlib::exceptions::null( L"LoadLibrary failed for " + pathname );
}

fostlib::dynlib::~dynlib() {
}

