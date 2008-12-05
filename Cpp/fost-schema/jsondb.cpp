/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema.hpp"
#include <fost/db.hpp>
#include <fost/thread.hpp>
#include <fost/exception/out_of_range.hpp>
#include <fost/exception/unexpected_eof.hpp>


using namespace fostlib;


namespace {


    in_process< json > &g_database() {
        static in_process< json > database( boost::shared_ptr< json >( new json( json::object_t() ) ) );
        return database;
    }


    const class jsonInterface : public dbinterface {
    public:
        jsonInterface();

        void create_database( dbconnection &dbc, const string &name ) const;
        void drop_database( dbconnection &dbc, const string &name ) const;

        boost::shared_ptr< dbinterface::read > reader( dbconnection &dbc ) const;
    } c_driver;


    class jsonreader : public dbinterface::read {
    public:
        jsonreader( dbconnection &d );

        boost::shared_ptr< dbinterface::recordset > query( const string &command ) const;
        boost::shared_ptr< dbinterface::write > writer();
    };


    class jsonwriter : public dbinterface::write {
    public:
        jsonwriter( dbinterface::read &reader );

        void create_table( const meta_instance &meta );
        void drop_table( const meta_instance &meta );
        void drop_table( const string &table );

        void execute( const string &cmd );
        void commit();
        void rollback();

    private:
        std::vector< boost::function< void ( json * ) > > m_operations;
    };


    class jsonrecordset : public dbinterface::recordset {
    public:
        jsonrecordset( const dbinterface &dbi, const string &cmd, const json &rs );

        bool eof() const;
        void moveNext();
        std::size_t fields() const;
        const string &name( std::size_t f ) const;
        const json &field( std::size_t i ) const;
        const json &field( const string &name ) const;

    private:
        uint64_t m_p;
        json m_rs;
        mutable json m_cache;
    };


}


/*
    jsonInterface
*/


jsonInterface::jsonInterface()
: dbinterface( L"json" ) {
}


#include <fost/exception/not_implemented.hpp>
void jsonInterface::create_database( dbconnection &/*dbc*/, const string &/*name*/ ) const {
    throw exceptions::not_implemented( L"void jsonInterface::create_database( dbconnection &dbc, const string &name ) const" );
}


void jsonInterface::drop_database( dbconnection &/*dbc*/, const string &/*name*/ ) const {
    throw exceptions::not_implemented( L"void jsonInterface::drop_database( dbconnection &dbc, const string &name ) const" );
}


boost::shared_ptr< dbinterface::read > jsonInterface::reader( dbconnection &dbc ) const {
    return boost::shared_ptr< dbinterface::read >( new jsonreader( dbc ) );
}


/*
    jsonreader
*/

jsonreader::jsonreader( dbconnection &dbc )
: read( dbc ) {
}


namespace {
    json dump( json &j ) {
        return json( j );
    }
}
boost::shared_ptr< dbinterface::recordset > jsonreader::query( const string &command ) const {
    if ( command == L"dump" ) {
        json p = g_database().synchronous< json >( boost::lambda::bind( dump, boost::lambda::_1 ) );
        if ( p.isnull() )
            throw exceptions::null( L"Null database dump received" );
        return boost::shared_ptr< dbinterface::recordset >( new jsonrecordset( m_connection.driver(), command, json( json::object_t() )
                .insert( L"meta", json().push_back( json() ) )
                .insert( L"data", json().push_back( json().push_back( json( json::unparse( p ) ) ) ) )
                ) );
    } else
        throw exceptions::not_implemented( L"boost::shared_ptr< dbinterface::recordset > jsonreader::recordset( const string &command ) const", command );
}


boost::shared_ptr< dbinterface::write > jsonreader::writer() {
    return boost::shared_ptr< dbinterface::write >( new jsonwriter( *this ) );
}


/*
    jsonwriter
*/


jsonwriter::jsonwriter( dbinterface::read &reader )
: dbinterface::write( reader ) {
}


void jsonwriter::create_table( const meta_instance &meta ) {
    m_operations.push_back( boost::lambda::bind( (json &(json::*)( const string &, const json & ))&json::insert, boost::lambda::_1, meta.fq_name(), json() ) );
}

void jsonwriter::drop_table( const meta_instance &/*meta*/ ) {
    throw exceptions::not_implemented( L"void jsonwriter::drop_table( const meta_instance &meta ) const" );
}
void jsonwriter::drop_table( const string &/*table*/ ) {
    throw exceptions::not_implemented( L"void jsonwriter::drop_table( const string &table ) const" );
}


void jsonwriter::execute( const string &/*cmd*/ ) {
    throw exceptions::not_implemented( L"void jsonwriter::execute( const string &cmd ) const" );
}


namespace {
    bool do_commit( json &j, const std::vector< boost::function< void ( json * ) > > &ops ) {
        for ( std::vector< boost::function< void ( json * ) > >::const_iterator op( ops.begin() ); op != ops.end(); ++op )
            (*op)( &j );
        return true;
    }
}
void jsonwriter::commit() {
    g_database().synchronous< bool >( boost::lambda::bind( do_commit, boost::lambda::_1, m_operations ) );
}


void jsonwriter::rollback() {
}


/*
    jsonrecordset
*/


jsonrecordset::jsonrecordset( const dbinterface &dbi, const string &cmd, const json &rs )
: dbinterface::recordset( dbi, cmd ), m_p( 0 ), m_rs( rs ) {
}


bool jsonrecordset::eof() const {
    throw exceptions::not_implemented( L"bool jsonrecordset::eof() const" );
}


void jsonrecordset::moveNext() {
    throw exceptions::not_implemented( L"void jsonrecordset::moveNext()" );
}


std::size_t jsonrecordset::fields() const {
    throw exceptions::not_implemented( L"std::size_t jsonrecordset::fields() const" );
}


const string &jsonrecordset::name( std::size_t /*f*/ ) const {
    throw exceptions::not_implemented( L"const string &jsonrecordset::name( std::size_t f ) const" );
}


const json &jsonrecordset::field( std::size_t i ) const {
    json r( m_rs );
    if ( i > r[ L"meta" ].size() )
        throw exceptions::out_of_range< int64_t >( L"Column index too large", 0, r[ L"meta" ].size(), i );
    else if ( m_p >= r[ L"data" ].size() )
        throw exceptions::unexpected_eof( L"End of recordset" );
    else {
        m_cache = r[ L"data" ][ m_p ][ i ];
        return m_cache;
    }
}


const json &jsonrecordset::field( const string &/*name*/ ) const {
    throw exceptions::not_implemented( L"const _variant_t &jsonrecordset::field( const string &name ) const" );
}

