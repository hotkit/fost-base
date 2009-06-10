/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_CACHE_HPP
#define FOST_CACHE_HPP
#pragma once


#include <fost/db>
#include <boost/thread.hpp>


namespace fostlib {


    template< typename object_type >
    class objectcache;

    template<>
    class FOST_CACHE_DECLSPEC objectcache< meta_instance > : boost::noncopyable {
        std::map< instance::key_type, boost::shared_ptr< instance > > m_objects;
    public:
        virtual ~objectcache();
    };

    template< typename object_type >
    class objectcache : objectcache< meta_instance > {
    };


    class FOST_CACHE_DECLSPEC fostcache : boost::noncopyable {
        static boost::thread_specific_ptr< fostcache > s_instance;
        std::map< boost::shared_ptr< meta_instance >, boost::shared_ptr< objectcache< meta_instance > > > m_caches;
    public:
        fostcache();
        ~fostcache();

        // Check the existence of a cache and return the instance associated with the current thread
        static bool exists();
        static fostcache &instance();

        fostcache &type( boost::shared_ptr< fostlib::meta_instance > type );
    };


}


#endif // FOST_CACHE_HPP
