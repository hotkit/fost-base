/*
    Copyright 1998-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-cache.hpp"
#include <fost/cache.hpp>
#include <fost/exception/not_implemented.hpp>


using namespace fostlib;


/*
    objectcache
*/

fostlib::objectcache< meta_instance >::~objectcache() {
}


/*
    fostcache
*/

fostlib::fostcache::fostcache() {
}

fostlib::fostcache::~fostcache() {
}

fostcache &fostlib::fostcache::type( boost::shared_ptr< fostlib::meta_instance > type ) {
    m_caches[ type ] = boost::shared_ptr< fostlib::objectcache< meta_instance > >( new fostlib::objectcache< meta_instance > );
    return *this;
}
