/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-jsondb.hpp"
#include <fost/jsondb.hpp>
#include <fost/unicode.hpp>

#include <fost/exception/not_null.hpp>
#include <fost/exception/out_of_range.hpp>
#include <fost/exception/unexpected_eof.hpp>


using namespace fostlib;


/*
    fostlib::jsondb
*/


namespace {
    void do_save( const json &j, const string &file ) {
        utf::save_file( coerce< utf8string >( file ).c_str(), json::unparse( j, false ) );
    }
    boost::shared_ptr< json > construct( const string &filename, const nullable< json > &default_db ) {
        boost::shared_ptr< json > ret( new json );
        try {
            *ret = json::parse( utf::load_file( coerce< utf8string >( filename ).c_str() ) );
        } catch ( exceptions::unexpected_eof & ) {
            if ( default_db.isnull() )
                throw;
            *ret = default_db.value();
            do_save( *ret, filename );
        }
        return ret;
    }
}


fostlib::jsondb::jsondb()
: m_blob( boost::shared_ptr< json >( new json ) ) {
}

fostlib::jsondb::jsondb( const string &filename, const nullable< json > &default_db )
: m_blob( boost::lambda::bind( construct, filename, default_db ) ), m_path( filename ) {
}


/*
    fostlib::jsondb::local
*/


namespace {
    json dump( json &j, const jcursor &p ) {
        return json( j[ p ] );
    }

    void do_insert( json &db, const jcursor &k, const json &v ) {
        if ( !db.has_key( k ) )
            k( db ) = v;
        else
            throw exceptions::not_null( L"There is already some JSON at this key position",
                json::unparse( db, true ) + L"\n" + json::unparse( v, true )
            );
    }

    json &do_commit( json &j, const jsondb::operations_type &ops ) {
        json db( j );
        try {
            for ( jsondb::operations_type::const_iterator op( ops.begin() ); op != ops.end(); ++op )
                (*op)( j );
        } catch ( ... ) {
            j = db;
            throw;
        }
        return j;
    }
}


fostlib::jsondb::local::local( jsondb &db, const jcursor &pos )
: m_db( db ), m_position( pos ) {
    refresh();
}

void fostlib::jsondb::local::refresh() {
    m_local = m_db.m_blob.synchronous< json >( boost::lambda::bind( dump, boost::lambda::_1, m_position ) );
}

jsondb::local &fostlib::jsondb::local::insert( const jcursor &position, const json &item ) {
    do_insert( m_local, position, item );
    m_operations.push_back( boost::lambda::bind( do_insert, boost::lambda::_1, position, item ) );
    return *this;
}

void fostlib::jsondb::local::commit() {
    if ( !m_db.m_path.isnull() )
        m_operations.push_back( boost::lambda::bind( do_save, boost::lambda::_1, m_db.m_path.value() ) );
    try {
        m_local = m_db.m_blob.synchronous< json >( boost::lambda::bind( do_commit, boost::lambda::_1, m_operations ) );
    } catch ( ... ) {
        rollback();
        throw;
    }
    m_operations.clear();
}

void fostlib::jsondb::local::rollback() {
    m_operations.clear();
    refresh();
}
