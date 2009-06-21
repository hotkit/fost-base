/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/atexit.hpp>

#include <boost/thread/once.hpp>


using namespace fostlib;


namespace {
    boost::once_flag &g_once_flag();

    typedef std::list< boost::function< void( void ) > > atexit_collection;
    atexit_collection &g_atexit_collection();

    void atexit_installer();
    void collection_executor();
}


void fostlib::atexit( boost::function< void( void ) > function ) {
    g_atexit_collection().push_back( function );
}


namespace {


    boost::once_flag &g_once_flag() {
        static boost::once_flag flag = BOOST_ONCE_INIT;
        return flag;
    }


    atexit_collection &g_atexit_collection() {
        static atexit_collection collection;
        boost::call_once( atexit_installer, g_once_flag() );
        return collection;
    }


    void atexit_installer() {
        std::atexit( collection_executor );
    }


    void collection_executor() {
        for ( atexit_collection::iterator f( g_atexit_collection().begin() ); f != g_atexit_collection().end(); ++f )
            (*f)();
    }


}
