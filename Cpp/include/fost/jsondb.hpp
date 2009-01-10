/*
    Copyright 2007-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_JSONDB_HPP
#define FOST_JSONDB_HPP
#pragma once


#include <fost/core>
#include <fost/json.hpp>
#include <fost/thread.hpp>


namespace fostlib {


    class FOST_JSONDB_DECLSPEC jsondb : boost::noncopyable {
        in_process< json > m_blob;
    public:
        typedef boost::function< void ( json & ) > operation_signature_type;
        typedef std::vector< operation_signature_type > operations_type;

        jsondb();
        explicit jsondb( const string &filename, const nullable< json > &default_db = null );

        accessors< const nullable< string > > filename;

        class FOST_JSONDB_DECLSPEC local : boost::noncopyable {
            jsondb &m_db;
            json m_local;
            const jcursor m_position;
            operations_type m_operations;
        public:
            explicit local( jsondb &db, const jcursor & = jcursor() );

            template< typename key >
            bool has_key( const key &k ) const {
                return m_local.has_key( k );
            }
            template< typename key >
            const json &operator [] ( const key &p ) const {
                return m_local[ p ];
            }

            local &insert( const jcursor &position, const json &item );
            local &update( const jcursor &position, const json &item );
            local &remove( const jcursor &position );

            template< typename V >
            local &insert( const jcursor &position, const V &item ) {
                return insert( position, json( item ) );
            }
            template< typename V >
            local &update( const jcursor &position, const V &item ) {
                return update( position, json( item ) );
            }

            void commit();
            void rollback();

        private:
            void refresh();
        };
        friend class local;
    };


}


#endif // FOST_JSONDB_HPP
