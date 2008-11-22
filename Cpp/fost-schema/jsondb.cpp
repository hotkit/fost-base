/*
	$Revision: $
	$Date: $
	Copyright (C) 2008, Felspar. Contact "http://fost.3.felspar.com".
*/


#include "stdafx.h"
#include "FOST.db.hpp"
#pragma warning ( disable : 4180 ) // qualifier applied to function type has no meaning; ignored
#include "FOST.task.hpp"


using namespace FSLib;


namespace {


    FSLib::Revision c_revision( L"$Archive: $", __DATE__, L"$Revision: $", L"$Date: $" );


    InProcess< Json > &g_database() {
        static InProcess< Json > database( boost::shared_ptr< Json >( new Json( Json::object_t() ) ) );
        return database;
    }


    const class JsonInterface : public FSLib::DBInterface {
    public:
        JsonInterface();

        void create_database( FSLib::DBConnection &dbc, const fostlib::string &name ) const;
        void drop_database( FSLib::DBConnection &dbc, const fostlib::string &name ) const;

        boost::shared_ptr< FSLib::DBInterface::Read > reader( FSLib::DBConnection &dbc ) const;
    } c_driver;


    class JsonReader : public FSLib::DBInterface::Read {
    public:
        JsonReader( FSLib::DBConnection &d );

        boost::shared_ptr< FSLib::DBInterface::Recordset > recordset( const fostlib::string &command ) const;
        boost::shared_ptr< FSLib::DBInterface::Write > writer();
    };


    class JsonWriter : public FSLib::DBInterface::Write {
    public:
        JsonWriter( FSLib::DBInterface::Read &reader );

        void create_table( const fostlib::string &table, const std::list< std::pair< fostlib::string, fostlib::string > > &key, const std::list< std::pair< fostlib::string, fostlib::string > > &columns );
        void drop_table( const fostlib::string &table );

        void execute( const fostlib::string &cmd );
        void commit();
        void rollback();

    private:
        std::vector< boost::function< void ( Json * ) > > m_operations;
    };


    class JsonRecordset : public FSLib::DBInterface::Recordset {
    public:
        JsonRecordset( const DBInterface &dbi, const fostlib::string &cmd, const Json &rs );

        bool eof() const;
        void moveNext();
        std::size_t fields() const;
        const fostlib::string &name( std::size_t f ) const;
        const Json &field( std::size_t i ) const;
        const Json &field( const fostlib::string &name ) const;

    private:
        int64_t m_p;
        Json m_rs;
        mutable Json m_cache;
    };


}


/*
    JsonInterface
*/


JsonInterface::JsonInterface()
: DBInterface( L"json", FSLib::Mangling::e_fost ) {
}


void JsonInterface::create_database( FSLib::DBConnection &/*dbc*/, const fostlib::string &/*name*/ ) const {
    throw FSLib::Exceptions::NotImplemented( L"void JsonInterface::create_database( FSLib::DBConnection &dbc, const fostlib::string &name ) const" );
}


void JsonInterface::drop_database( FSLib::DBConnection &/*dbc*/, const fostlib::string &/*name*/ ) const {
    throw FSLib::Exceptions::NotImplemented( L"void JsonInterface::drop_database( FSLib::DBConnection &dbc, const fostlib::string &name ) const" );
}


boost::shared_ptr< FSLib::DBInterface::Read > JsonInterface::reader( FSLib::DBConnection &dbc ) const {
    return boost::shared_ptr< FSLib::DBInterface::Read >( new JsonReader( dbc ) );
}


/*
    JsonReader
*/

JsonReader::JsonReader( FSLib::DBConnection &dbc )
: Read( dbc ) {
}


namespace {
    Json dump( Json &j ) {
        return Json( j );
    }
}
boost::shared_ptr< FSLib::DBInterface::Recordset > JsonReader::recordset( const fostlib::string &command ) const {
    if ( command == L"dump" ) {
        Json p = g_database().synchronous< Json >( boost::lambda::bind( dump, boost::lambda::_1 ) );
        if ( p.isnull() )
            throw FSLib::Exceptions::Null( L"Null database dump received" );
        return boost::shared_ptr< FSLib::DBInterface::Recordset >( new JsonRecordset( m_connection.dbInterface(), command, Json( Json::object_t() )
                .insert( L"meta", Json().push_back( Json() ) )
                .insert( L"data", Json().push_back( Json().push_back( Json( toJson( p ) ) ) ) )
                ) );
    } else
        throw FSLib::Exceptions::NotImplemented( L"boost::shared_ptr< FSLib::DBInterface::Recordset > JsonReader::recordset( const fostlib::string &command ) const", command );
}


boost::shared_ptr< FSLib::DBInterface::Write > JsonReader::writer() {
    return boost::shared_ptr< FSLib::DBInterface::Write >( new JsonWriter( *this ) );
}


/*
    JsonWriter
*/


JsonWriter::JsonWriter( FSLib::DBInterface::Read &reader )
: FSLib::DBInterface::Write( reader ) {
}


void JsonWriter::create_table( const fostlib::string &table, const std::list< std::pair< fostlib::string, fostlib::string > > &/*key*/, const std::list< std::pair< fostlib::string, fostlib::string > > &/*columns*/ ) {
    m_operations.push_back( boost::lambda::bind( (Json &(Json::*)( const fostlib::string &, const Json & ))&Json::insert, boost::lambda::_1, table, Json() ) );
}


void JsonWriter::drop_table( const fostlib::string &/*table*/ ) {
    throw FSLib::Exceptions::NotImplemented( L"void JsonWriter::drop_table( const fostlib::string &table ) const" );
}


void JsonWriter::execute( const fostlib::string &/*cmd*/ ) {
    throw FSLib::Exceptions::NotImplemented( L"void JsonWriter::execute( const fostlib::string &cmd ) const" );
}


namespace {
    bool do_commit( Json &j, const std::vector< boost::function< void ( Json * ) > > &ops ) {
        for ( std::vector< boost::function< void ( Json * ) > >::const_iterator op( ops.begin() ); op != ops.end(); ++op )
            (*op)( &j );
        return true;
    }
}
void JsonWriter::commit() {
    g_database().synchronous< bool >( boost::lambda::bind( do_commit, boost::lambda::_1, m_operations ) );
}


void JsonWriter::rollback() {
}


/*
    JsonRecordset
*/


JsonRecordset::JsonRecordset( const DBInterface &dbi, const fostlib::string &cmd, const Json &rs )
: FSLib::DBInterface::Recordset( dbi, cmd ), m_rs( rs ), m_p( 0 ) {
}


bool JsonRecordset::eof() const {
    throw FSLib::Exceptions::NotImplemented( L"bool JsonRecordset::eof() const" );
}


void JsonRecordset::moveNext() {
    throw FSLib::Exceptions::NotImplemented( L"void JsonRecordset::moveNext()" );
}


std::size_t JsonRecordset::fields() const {
    throw FSLib::Exceptions::NotImplemented( L"std::size_t JsonRecordset::fields() const" );
}


const fostlib::string &JsonRecordset::name( std::size_t /*f*/ ) const {
    throw FSLib::Exceptions::NotImplemented( L"const fostlib::string &JsonRecordset::name( std::size_t f ) const" );
}


const Json &JsonRecordset::field( std::size_t i ) const {
    Json r( m_rs );
    if ( i > r[ L"meta" ].size() )
        throw FSLib::Exceptions::OutOfRange< int64_t >( L"Column index too large", 0, r[ L"meta" ].size(), i );
    else if ( m_p >= r[ L"data" ].size() )
        throw FSLib::Exceptions::UnexpectedEOF( L"End of recordset" );
    else {
        m_cache = r[ L"data" ][ m_p ][ i ];
        return m_cache;
    }
}


const Json &JsonRecordset::field( const fostlib::string &/*name*/ ) const {
    throw FSLib::Exceptions::NotImplemented( L"const _variant_t &JsonRecordset::field( const fostlib::string &name ) const" );
}

