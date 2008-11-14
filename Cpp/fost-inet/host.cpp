/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-inet.hpp"
#include <fost/host.hpp>
#include <fost/exception/not_implemented.hpp>


using namespace fostlib;


namespace {



	ipv4address addressFromName( const fostlib::string &name ) {
        throw fostlib::exceptions::not_implemented( L"addressFromName" );
	}


	fostlib::string nameFromAddress( unsigned long int address ) {
		stringstream s;
		s << ( ( address & 0xff000000 ) >> 24 ) << L"." << ( ( address & 0x00ff0000 ) >> 16 ) << L"." <<
				( ( address & 0x0000ff00 ) >> 8 ) << L"." << ( address & 0x000000ff );
		return fostlib::string( s.str() );
	}


}


fostlib::host::host()
: m_name(), m_address( 0 ) {
}


fostlib::host::host( const fostlib::string &name )
: m_name( name ), m_address( 0 ) {
}


fostlib::host::host( ipv4address address )
: m_name( nameFromAddress( address ) ), m_address( address ) {
}


fostlib::host::host( unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4 )
: m_name(), m_address( ( b1 << 24 ) + ( b2 << 16 ) + ( b3 << 8 ) + b4 ) {
	m_name = nameFromAddress( m_address );
}


bool fostlib::host::operator ==( const host &h ) const {
	return name() == h.name();
}
bool fostlib::host::operator !=( const host &h ) const {
	return name() != h.name();
}


ipv4address fostlib::host::address() const {
	if ( m_address == 0 )
		m_address = addressFromName( m_name );
	return m_address;
}


fostlib::string fostlib::host::name() const {
	return m_name;
}

