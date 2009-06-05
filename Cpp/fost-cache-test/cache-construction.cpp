/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-cache-test.hpp"
#include <fost/cache>
#include <fost/exception/not_null.hpp>
#include <fost/exception/null.hpp>


FSL_TEST_SUITE( construction );


FSL_TEST_FUNCTION( objectcache ) {
    fostlib::objectcache< fostlib::meta_instance > cache;
}


FSL_TEST_FUNCTION( fostcache ) {
    // Asking for the instance before we create one throws a null exception
    FSL_CHECK_EXCEPTION( fostlib::fostcache::instance(), fostlib::exceptions::null& );

    fostlib::fostcache cache;

    // Creating a second one throws a not null exception
    FSL_CHECK_EXCEPTION( fostlib::fostcache cache2, fostlib::exceptions::not_null& );
}


FSL_TEST_FUNCTION( type_registration ) {
    // We can now register types either through the stack reference or via the instance()
    // Registering a type is idempotent
    boost::shared_ptr< fostlib::meta_instance > type;
    cache.type( type );
    fostlib::fostcache::instance().type( type );
}
