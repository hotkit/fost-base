/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema.hpp"
#include <fost/db.hpp>
#include <fost/schema.hpp>
#include <fost/thread.hpp>
#include <fost/exception/out_of_range.hpp>
#include <fost/exception/unexpected_eof.hpp>
#include <fost/exception/query_failure.hpp>


using namespace fostlib;


namespace {


    in_process< json > &g_database( string dbname ) {
        static boost::mutex mx;
        static std::map< string, boost::shared_ptr< in_process< json > > > databases;
        boost::mutex::scoped_lock lock( mx );
        std::map< string, boost::shared_ptr< in_process< json > > >::iterator p( databases.find( dbname ) );
        if ( p == databases.end() ) {
             boost::shared_ptr< json > db_template( new json( json::object_t() ) );
            if ( dbname == L"master" ) {
                db_template->insert( L"database", json::object_t() );
            }
            p = databases.insert( std::make_pair( dbname, boost::shared_ptr< in_process< json > >(
                new in_process< json >( db_template )
            ) ) ).first;
        }
        return *p->second;
    }
    in_process< json > &g_database( dbconnection &dbc ) {
        if ( !dbc.writeDSN().isnull() && dbc.writeDSN() != dbc.readDSN() )
            throw exceptions::data_driver( L"JSON database must have the same read/write connections", L"json" );
        return g_database( dbc.readDSN() );
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

        boost::shared_ptr< dbinterface::recordset > query( const meta_instance &item, const json &key ) const;
        boost::shared_ptr< dbinterface::write > writer();

        boost::scoped_ptr< json > data;
        in_process< json > &database;
    };


    class jsonwriter : public dbinterface::write {
    public:
        jsonwriter( jsonreader &reader );

        void create_table( const meta_instance &meta );
        void drop_table( const meta_instance &meta );
        void drop_table( const string &table );

        void insert( const instance &object );
        void commit();
        void rollback();

        in_process< json > &database;

    private:
        std::vector< boost::function< void ( json * ) > > m_operations;
    };


    class jsonrecordset : public dbinterface::recordset {
    public:
        jsonrecordset( const json &rs );

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

namespace {
    struct master {
        master()
        : database( L"database" ) {
            database.primary_key( L"name", L"text" );
        }
        meta_instance database;
    };
    boost::scoped_ptr< master > master_schema;
}

jsonInterface::jsonInterface()
: dbinterface( L"json" ) {
}

void jsonInterface::create_database( dbconnection &dbc, const string &name ) const {
    if ( !master_schema ) master_schema.reset( new master );
    if ( dbc.readDSN() != L"master" )
        throw exceptions::data_driver( L"Can only create tables when connected to the 'master' database", L"json" );
    fostlib::recordset rs( dbc.query( master_schema->database, json( name ) ) );
    if ( rs.eof() ) {
        g_database( name );
        boost::shared_ptr< instance > dbrep( master_schema->database.create( dbc, json( name ) ) );
        dbtransaction trans( dbc );
        dbrep->save();
        trans.commit();
    } else
        throw exceptions::query_failure(
            L"The requested database already exists", master_schema->database
        );
}

#include <fost/exception/not_implemented.hpp>
void jsonInterface::drop_database( dbconnection &/*dbc*/, const string &/*name*/ ) const {
    if ( !master_schema ) master_schema.reset( new master );
    throw exceptions::not_implemented( L"void jsonInterface::drop_database( dbconnection &dbc, const string &name ) const" );
}

boost::shared_ptr< dbinterface::read > jsonInterface::reader( dbconnection &dbc ) const {
    if ( !master_schema ) master_schema.reset( new master );
    return boost::shared_ptr< dbinterface::read >( new jsonreader( dbc ) );
}


/*
    jsonreader
*/

namespace {
    json dump( json &j ) {
        return json( j );
    }
}
jsonreader::jsonreader( dbconnection &dbc )
: read( dbc ), database( g_database( dbc ) ) {
    data.reset( new json(
        database.synchronous< json >( boost::lambda::bind( dump, boost::lambda::_1 ) )
    ) );
}


boost::shared_ptr< dbinterface::recordset > jsonreader::query( const meta_instance &item, const json &key ) const {
    if ( m_connection.in_transaction() || !data )
        throw exceptions::transaction_fault(
            L"Cannot query the JSON database whilst there is a write transaction"
        );
    if ( data->has_key( item.fq_name() ) ) {
        if ( key.isnull() )
            return boost::shared_ptr< dbinterface::recordset >(
                new jsonrecordset( (*data)[ item.fq_name() ] )
            );
        return boost::shared_ptr< dbinterface::recordset >(
            new jsonrecordset(
                (*data)[ item.fq_name() ][ coerce< string >( key ) ]
            )
        );
    } else
        throw exceptions::query_failure( L"No database table found", item );
}


boost::shared_ptr< dbinterface::write > jsonreader::writer() {
    return boost::shared_ptr< dbinterface::write >( new jsonwriter( *this ) );
}


/*
    jsonwriter
*/


jsonwriter::jsonwriter( jsonreader &reader )
: dbinterface::write( reader ), database( reader.database ) {
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


void jsonwriter::insert( const instance &object ) {
    jcursor key = jcursor()[ object._meta().fq_name() ];
    json repr = json::object_t();
    for ( meta_instance::const_iterator col( object._meta().begin() ); col != object._meta().end(); ++col ) {
        if ( (*col)->key() )
            key = key[ object[ (*col)->name() ].to_json() ];
        repr.insert( (*col)->name(), object[ (*col)->name() ].to_json() );
    }
    throw exceptions::not_implemented( L"void jsonwriter::insert( const instance &object, boost::function< void( void ) > oncommit )" );
}


namespace {
    bool do_commit( json &j, const std::vector< boost::function< void ( json * ) > > &ops ) {
        for ( std::vector< boost::function< void ( json * ) > >::const_iterator op( ops.begin() ); op != ops.end(); ++op )
            (*op)( &j );
        return true;
    }
}
void jsonwriter::commit() {
    database.synchronous< bool >( boost::lambda::bind( do_commit, boost::lambda::_1, m_operations ) );
}


void jsonwriter::rollback() {
}


/*
    jsonrecordset
*/


jsonrecordset::jsonrecordset( const json &rs )
: dbinterface::recordset( L"JSON query" ), m_p( 0 ), m_rs( rs ) {
}


bool jsonrecordset::eof() const {
    if ( m_rs.isnull() ) return true;
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

