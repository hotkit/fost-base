/*
    Copyright 2008-2012, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <dlfcn.h>
#include <fost/atexit.hpp>


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
: m_lib(nullptr) {
    void *handle;
    string munged("lib" + lib);
#ifdef _DEBUG
    munged += "-d";
#endif
#if defined(FOST_OS_LINUX)
    munged += ".so";
#elif defined(FOST_OS_OSX)
    munged += ".dylib";
#endif
    if ( (handle = dlopen(munged.c_str(), RTLD_NOW)) == nullptr ) {
        if ( (handle = dlopen(lib.c_str(), RTLD_NOW)) == nullptr ) {
            throw fostlib::exceptions::null("dlopen failed for " + munged + " and " + lib, string(dlerror()));
        }
    }
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
    if ( m_lib ) fostlib::atexit([libp = m_lib]() { delete libp; });
}
