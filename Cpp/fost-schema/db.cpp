/*
	$Revision: 47 $
	$Date: 28/08/05 21:37 $
	Copyright (C) 1999-2008, Felspar. Contact "http://fost.3.felspar.com".
*/


#include "stdafx.h"
#include <FOST.db.hpp>
#include "FOST.kpi.hpp"
#include "FOST.task.hpp"


using namespace std;
using namespace FSLib;
using namespace FSLib::Exceptions;


namespace {


	Revision c_revision( L"$Archive: /FOST.3/F3Util/db.cpp $", __DATE__, L"$Revision: 47 $", L"$Date: 28/08/05 21:37 $" );
	Revision c_header( FOST_DB_HPP_ARCHIVE, __DATE__, FOST_DB_HPP_REVISION, FOST_DB_HPP_DATE );


    const Setting< fostlib::string > c_defaultDriver( L"$Archive: /FOST.3/F3Util/db.cpp $", L"Database", L"Default driver", L"ado", true );

#ifdef _DEBUG
#define LOGGING true
#else
#define LOGGING false
#endif
	const Setting< bool > c_logConnect( L"$Archive: /FOST.3/F3Util/db.cpp $", L"Database", L"LogConnect", LOGGING, true );
	const Setting< bool > c_logRead( L"$Archive: /FOST.3/F3Util/db.cpp $", L"Database", L"LogRead", LOGGING, true );
	const Setting< bool > c_logWrite( L"$Archive: /FOST.3/F3Util/db.cpp $", L"Database", L"LogWrite", LOGGING, true );
	const Setting< bool > c_logFailure( L"$Archive: /FOST.3/F3Util/db.cpp $", L"Database", L"LogFailure", true, true );
#undef LOGGING

	const Setting< long > c_countCommandTimeout( L"$Archive: /FOST.3/F3Util/db.cpp $", L"Database", L"CountCommandTimeout", 120, true );
	const Setting< long > c_readCommandTimeout( L"$Archive: /FOST.3/F3Util/db.cpp $", L"Database", L"ReadCommandTimeout", 3600, true );
	const Setting< long > c_writeCommandTimeout( L"$Archive: /FOST.3/F3Util/db.cpp $", L"Database", L"WriteCommandTimeout", 15, true );


    FSLib::Library< const FSLib::DBInterface * > &g_interfaces() {
        static FSLib::Library< const FSLib::DBInterface * > interfaces;
        return interfaces;
    }

    fostlib::string driver_name( const fostlib::string &d ) {
        fostlib::string p( FSLib::partition( d, L";" ).first );
        fostlib::string::size_type s( p.find( L'/' ) );
        if ( s == std::fostlib::string::npos )
            return fostlib::string();
        else
            return fostlib::string( p, 0, s );
    }
    fostlib::string dsn( const fostlib::string &d ) {
        fostlib::string n( driver_name( d ) );
        if ( n.empty() )
            return d;
        else
            return d.substr( n.length() + 1 );
    }
    fostlib::string driver( const fostlib::string &read, const fostlib::nullable< fostlib::string > &write ) {
        fostlib::string rd( driver_name( read ) ), wd( driver_name( write.value( read ) ) );
        if ( rd != wd )
            throw FSLib::Exceptions::DataDriver( L"Read and write drivers not the same", rd, wd );
        if ( rd.empty() )
            return c_defaultDriver.value();
        else
            return rd;
    }


}


/*
    DBInterface
*/


const FSLib::DBInterface &FSLib::DBInterface::connection( const fostlib::string &read, const fostlib::nullable< fostlib::string > &write ) {
    if ( g_interfaces().find( driver( read, write ) ).empty() )
        throw FSLib::Exceptions::DataDriver( L"No driver found", driver( read, write ) );
    return **g_interfaces().find( driver( read, write ) ).begin();
}


FSLib::DBInterface::DBInterface( const fostlib::string &name, Mangling::t_translation t )
: translation( t ) {
    g_interfaces().add( name, this );
}


FSLib::DBInterface::Read::Read( DBConnection &d )
: m_connection( d ) {
}


FSLib::DBInterface::Read::~Read() {
}


FSLib::DBInterface::Write::Write( FSLib::DBInterface::Read &r )
: m_connection( r.m_connection ), m_reader( r ) {
}


FSLib::DBInterface::Write::~Write() {
}


FSLib::DBInterface::Recordset::Recordset( const FSLib::DBInterface &dbi, const fostlib::string &c )
: translation( dbi.translation() ), command( c ) {
}


FSLib::DBInterface::Recordset::~Recordset() {
}


/*
	DBConnection
*/


const Setting< bool > DBConnection::c_commitCount( L"$Archive: /FOST.3/F3Util/db.cpp $", L"Database", L"Commit count", true, true );
const Setting< long > DBConnection::c_commitCountDomain( L"$Archive: /FOST.3/F3Util/db.cpp $", L"Database", L"Commit count id", 9, true );


DBConnection::DBConnection( const fostlib::string &r )
: m_transaction( NULL ), m_readOnly( true ), readDSN( dsn( r ) ),
        m_interface( DBInterface::connection( r, Null ) ) {
    m_connection = m_interface.reader( *this );
}


DBConnection::DBConnection( const fostlib::string &r, const fostlib::string &w )
: m_transaction( NULL ), m_readOnly( false ), readDSN( dsn( r ) ), writeDSN( dsn( w ) ),
		m_interface( DBInterface::connection( r, w ) ) {
    m_connection = m_interface.reader( *this );
}


DBConnection::~DBConnection()
try{
} catch ( ... ) {
	try {
		if ( Setting< bool >::value( L"Database", L"LogFailure" ) ) {
			YAML::Record failure;
			failure.add( L"DB", L"Failure" );
			failure.add( L"Place", L"DBConnection::~DBConnection()" );
			failure.add( L"Exception", L"Unknown exception" );
			failure.add( L"Connection", L"Read" );
			failure.log();
		}
	} catch ( exception & ) {
		absorbException();
	}
}


void DBConnection::create_database( const fostlib::string &name ) {
    if ( writeDSN().isnull() )
        throw FSLib::Exceptions::TransactionFault( L"Cannot create database without a write connection" );
    m_interface.create_database( *this, name );
}
void DBConnection::drop_database( const fostlib::string &name ) {
    if ( writeDSN().isnull() )
        throw FSLib::Exceptions::TransactionFault( L"Cannot drop database without a write connection" );
    m_interface.drop_database( *this, name );
}


const FSLib::DBInterface &DBConnection::dbInterface() const {
    return m_interface;
}


Recordset DBConnection::recordset( const fostlib::string &command ) {
    if ( !m_connection )
        throw FSLib::Exceptions::TransactionFault( L"Database connection has not started a read transaction" );
    return m_connection->recordset( command );
}


bool DBConnection::inTransaction() const {
    return m_transaction != NULL;
}


Transaction &DBConnection::transaction() {
    return *m_transaction;
}


/*
    Transaction
*/


FSLib::Transaction::Transaction( DBConnection &dbc )
: m_connection( dbc ) {
    if ( m_connection.inTransaction() )
        throw FSLib::Exceptions::TransactionFault( L"Nested transaction not yet supported" );
    m_transaction = dbc.m_connection->writer();
    m_connection.m_transaction = this;
}


FSLib::Transaction::~Transaction() {
    if ( m_transaction )
        m_transaction->rollback();
    if ( m_connection.m_transaction != this )
        throw FSLib::Exceptions::TransactionFault( L"The current transaction has changed" );
    m_connection.m_transaction = NULL;
}


void FSLib::Transaction::create_table( const fostlib::string &table, const std::list< std::pair< fostlib::string, fostlib::string > > &key, const std::list< std::pair< fostlib::string, fostlib::string > > &columns ) {
    if ( !m_transaction )
        throw FSLib::Exceptions::TransactionFault( L"This transaction has already been used" );
    m_transaction->create_table( table, key, columns );
}


void FSLib::Transaction::drop_table( const fostlib::string &table ) {
    if ( !m_transaction )
        throw FSLib::Exceptions::TransactionFault( L"This transaction has already been used" );
    m_transaction->drop_table( table );
}


Transaction &FSLib::Transaction::execute( const fostlib::string &c ) {
    if ( !m_transaction )
        throw FSLib::Exceptions::TransactionFault( L"This transaction has already been used" );
    m_transaction->execute( c );
    return *this;
}


void FSLib::Transaction::commit() {
    m_transaction->commit();
    m_transaction = boost::shared_ptr< DBInterface::Write >();
}


/*
	Recordset
*/


Recordset::Recordset( boost::shared_ptr< DBInterface::Recordset > rs )
: m_interface( rs ), translation( rs->translation() ) {
}


Recordset::~Recordset()
try {
} catch ( ... ) {
	absorbException();
}


bool Recordset::eof() const {
    return m_interface->eof();
}


bool Recordset::isnull( const fostlib::string &i ) const {
	return field( i ).isnull();
}


bool Recordset::isnull( std::size_t i ) const {
	return field( i ).isnull();
}


void Recordset::moveNext() {
    m_interface->moveNext();
}


std::size_t Recordset::fields() const {
	return m_interface->fields();
}


const fostlib::string &Recordset::name( std::size_t i ) const {
    return m_interface->name( i );
}


const Json &Recordset::field( std::size_t i ) const {
    if ( m_interface.get() )
        return m_interface->field( i );
    else
		throw Exceptions::UnexpectedEOF( L"The recordset came from a write SQL instruction" );
}


const Json &Recordset::field( const fostlib::string &name ) const {
	if ( m_interface.get() )
        return m_interface->field( name );
	else
		throw Exceptions::UnexpectedEOF( L"The recordset came from a write SQL instruction" );
}
