/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <dlfcn.h>

using namespace fostlib;


struct fostlib::dynlib::impl {
    void *handle;
};


fostlib::dynlib::dynlib( const string &lib )
: m_lib( new impl ) {
    if ( ( m_lib->handle = dlopen( lib.c_str(), RTLD_NOW ) ) == NULL )
        throw fostlib::exceptions::null( L"dlopen failed for " + lib, string( dlerror() ) );
}


fostlib::dynlib::~dynlib() {
    if ( m_lib ) {
        if ( m_lib->handle )
            dlclose( m_lib->handle );
        delete m_lib;
    }
}
