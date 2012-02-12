/*
    Copyright 1997-2012, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_THREADSAFE_STORE_HPP
#define FOST_THREADSAFE_STORE_HPP
#pragma once


#include <fost/string.hpp>
#include <fost/nullable.hpp>

#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-variable"
#endif

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

#ifdef __clang__
    #pragma clang diagnostic pop
#endif


#ifdef FOST_OS_WINDOWS
#    pragma warning ( push )
// C4244: '=' : conversion from '__w64 int' to 'unsigned int', possible loss of data
#    pragma warning ( disable : 4244 )
// C4267: 'return' : conversion from 'size_t' to 'int', possible loss of data
#    pragma warning ( disable : 4267 )
// C4311: 'type cast' : pointer truncation from 'void *const ' to 'long'
#    pragma warning ( disable : 4311 )
// C4312: 'type cast' : conversion from 'long' to 'void *' of greater size
#    pragma warning ( disable : 4312 )
#endif
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>
#ifdef FOST_OS_WINDOWS
#    pragma warning ( pop )
#endif


namespace fostlib {


    /// A thread safe data structure generally used to handle global data that comes and goes, e.g. plug ins.
    template< typename F, typename K = fostlib::string, typename S = std::multimap< K, F > >
    class threadsafe_store {
    public:
        typedef F item_t;
        typedef K key_t;
        typedef S store_t;
        typedef std::vector< key_t > keys_t;
        typedef std::vector< item_t > found_t;

        void add( const key_t &k, const item_t &f ) {
            //ExclusiveWrite::WriteLock lock( m_mutex );
            boost::mutex::scoped_lock lock( m_mutex );
            m_store.insert( std::make_pair( k, f ) );
        }
        bool remove( const key_t &k ) {
            boost::mutex::scoped_lock lock( m_mutex );
            return m_store.erase( k );
        }
        bool remove( const key_t &k, const item_t &f ) {
            boost::mutex::scoped_lock lock( m_mutex );
            for ( typename S::iterator i( m_store.lower_bound( k ) ); i != m_store.upper_bound( k ); ++i )
                if ( i->second == f ) {
                    m_store.erase( i );
                    return true;
                }
            return false;
        }
        found_t find( const key_t &k ) {
            //ExclusiveWrite::ReadLock lock( m_mutex );
            boost::mutex::scoped_lock lock( m_mutex );
            found_t found;
            std::transform(
                    m_store.lower_bound( k ), m_store.upper_bound( k ),
                    std::back_inserter( found ),
                    boost::lambda::bind( &store_t::value_type::second, boost::lambda::_1 ) );
            return found;
        }
        keys_t keys() {
            //ExclusiveWrite::ReadLock lock( m_mutex );
            boost::mutex::scoped_lock lock( m_mutex );
            keys_t all;
            all.reserve( m_store.size() );
            for ( typename store_t::const_iterator i( m_store.begin() ); i != m_store.end(); ++i )
                all.push_back( i->first );
            return all;
        }
    private:
        store_t m_store;
        boost::mutex m_mutex;
        //ExclusiveWrite m_mutex;
    };


}


#endif // FOST_THREADSAFE_STORE_HPP

