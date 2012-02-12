/*
    Copyright 2008-2012, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <dlfcn.h>
#include <fost/atexit.hpp>

#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-variable"
#endif

#include <boost/lambda/bind.hpp>
#include <boost/lambda/construct.hpp>

#ifdef __clang__
    #pragma clang diagnostic pop
#endif


using namespace fostlib;


struct fostlib::dynlib::impl {
    impl( void *h )
    : handle( h ) {
    }
    ~impl() {
        if ( handle )
            dlclose( handle );
    }
    string name;
    void *handle;
};


fostlib::dynlib::dynlib( const string &lib )
: m_lib( NULL ) {
    void *handle;
    if ( ( handle = dlopen( lib.c_str(), RTLD_NOW ) ) == NULL )
        throw fostlib::exceptions::null( L"dlopen failed for " + lib, string( dlerror() ) );
    m_lib = new fostlib::dynlib::impl(handle);
    m_lib->name = lib;
}


fostlib::dynlib::~dynlib() {
    /*
        We don't want to actually unload the .so yet as there may well still be objects
        that it manages.
        The choice of adding the deletion to atexit here is somewhat arbitrary. It could
        have just as easily gone in the constructor
    */
    if ( m_lib )
        fostlib::atexit( boost::lambda::bind( boost::lambda::delete_ptr(), m_lib ) );
}
