/*
	$Revision: 47 $
	$Date: 28/08/05 21:37 $
	Copyright (C) 1999-2008, FSL Technologies Limited. Contact "http://fost.3.felspar.com".
*/


#include "stdafx.h"
#include <FOST.db.hpp>


using namespace std;
using namespace FSLib;
using namespace FSLib::Exceptions;


namespace {


	Revision c_revision( L"$Archive: /FOST.3/F3Util/db.exception.cpp $", __DATE__, L"$Revision: 47 $", L"$Date: 28/08/05 21:37 $" );


	const wchar_t DataDriverMsg[]=L"Problem with database driver.";
	const wchar_t TransactionFaultMsg[]=L"Fault in transaction handling.";
	const wchar_t StringDecodeMsg[]=L"String decode error.";
	const wchar_t FieldCastFaultMsg[]=L"field_cast< T >() fault.";


}


/*
	Exceptions
*/


FSLib::Exceptions::DataDriver::DataDriver( const fostlib::string &m, const fostlib::string &d )
: Exception( m ) {
    m_info << L"Driver: " << d << L"\n";
}
FSLib::Exceptions::DataDriver::DataDriver( const fostlib::string &m, const fostlib::string &d1, const fostlib::string &d2 )
: Exception( m ) {
    m_info << L"Driver 1: " << d1 << L"\nDriver 2: " << d2 << L"\n";
}
const wchar_t * const FSLib::Exceptions::DataDriver::message() const {
	return DataDriverMsg;
}


FSLib::Exceptions::StringDecode::StringDecode( const fostlib::string &str )
: Exception(), string( str ) {
	m_info << string() << endl;
}
const wchar_t * const FSLib::Exceptions::StringDecode::message() const {
	return StringDecodeMsg;
}


FSLib::Exceptions::TransactionFault::TransactionFault( const fostlib::string &error )
: Exception( error ) {
}
const wchar_t * const FSLib::Exceptions::TransactionFault::message() const {
	return TransactionFaultMsg;
}


FSLib::Exceptions::FieldCastFault::FieldCastFault( const fostlib::string &error )
: Exception( error ) {
}
const wchar_t * const FSLib::Exceptions::FieldCastFault::message() const {
	return FieldCastFaultMsg;
}


