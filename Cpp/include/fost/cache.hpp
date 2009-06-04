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


namespace fostlib {


    template< typename object_type >
    class objectcache;

    template<>
    class FOST_CACHE_DECLSPEC objectcache< meta_instance > : boost::noncopyable {
    public:
        virtual ~objectcache();
    };

    template< typename object_type >
    class objectcache : objectcache< meta_instance > {
    };


    class FOST_CACHE_DECLSPEC fostcache : boost::noncopyable {
        std::set< boost::shared_ptr< objectcache< meta_instance > > > m_caches;
    public:
        fostcache();
    };


}


#endif // FOST_CACHE_HPP
