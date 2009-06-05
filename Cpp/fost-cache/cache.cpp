/*
    Copyright 1998-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-cache.hpp"
#include <fost/cache.hpp>
#include <fost/exception/not_implemented.hpp>
#include <fost/exception/not_null.hpp>
#include <fost/exception/null.hpp>


using namespace fostlib;


/*
    objectcache
*/

fostlib::objectcache< meta_instance >::~objectcache() {
}


/*
    fostcache
*/
namespace {
    void do_nothing(fostcache*) {}
}
boost::thread_specific_ptr< fostcache > fostlib::fostcache::s_instance( do_nothing );

fostlib::fostcache::fostcache() {
    if ( s_instance.get() )
        throw exceptions::not_null( L"There is already a fostcache in this thread" );
    else
        s_instance.reset( this );
}

fostlib::fostcache::~fostcache() {
    if ( s_instance.get() == this )
        s_instance.reset();
}

bool fostlib::fostcache::exists() {
    return s_instance.get();
}
fostcache &fostlib::fostcache::instance() {
    if ( !s_instance.get() )
        throw exceptions::null( L"There is no fostcache in this thread" );
    return *s_instance;
}

fostcache &fostlib::fostcache::type( boost::shared_ptr< fostlib::meta_instance > type ) {
    if ( m_caches.find( type ) == m_caches.end() )
        m_caches[ type ] = boost::shared_ptr< fostlib::objectcache< meta_instance > >( new fostlib::objectcache< meta_instance > );
    return *this;
}
