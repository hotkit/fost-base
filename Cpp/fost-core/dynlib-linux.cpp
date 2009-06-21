/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <dlfcn.h>
#include <fost/atexit.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/construct.hpp>


using namespace fostlib;


struct fostlib::dynlib::impl {
    impl()
    : handle( NULL ) {
    }
    ~impl() {
        if ( handle )
            dlclose( handle );
    }
    string name;
    void *handle;
};


fostlib::dynlib::dynlib( const string &lib )
: m_lib( new impl ) {
    m_lib->name = lib;
    if ( ( m_lib->handle = dlopen( lib.c_str(), RTLD_NOW ) ) == NULL )
        throw fostlib::exceptions::null( L"dlopen failed for " + lib, string( dlerror() ) );
}


fostlib::dynlib::~dynlib() {
    /*
        We don't want to actually unload the .so yet as there may well still be objects
        that it manages.
        The choice of adding the deletion to atexit here is somewhat arbitrary. It could
        have just as easily gone in the constructor
    */
    fostlib::atexit( boost::lambda::bind( boost::lambda::delete_ptr(), m_lib ) );
}
