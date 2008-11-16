/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-inet.hpp"
#include <fost/host.hpp>
#include <fost/parse/host.hpp>


using namespace fostlib;


const host_parser fostlib::host_p;


namespace {



    boost::asio::ip::address getaddr( const host &host ) {
        boost::asio::io_service io_service;
        boost::asio::ip::tcp::resolver resolver( io_service );
        boost::asio::ip::tcp::resolver::query query(
            coerce< std::string >( host.name() ), coerce< std::string >( host.service() )
        );
        boost::system::error_code error;
        boost::asio::ip::tcp::resolver::iterator it( resolver.resolve( query, error ) );
        if ( error == boost::asio::error::host_not_found )
            throw exceptions::host_not_found( host.name() );
        return it->endpoint().address();
    }


}


fostlib::host::host()
: m_service( L"0" ) {
}


fostlib::host::host( const fostlib::string &name, const nullable< string > &service )
: m_name( name ), m_service( service.value( L"0" ) ) {
}


fostlib::host::host( uint32_t address, const nullable< string > &service )
: m_service( service.value( L"0" ) ), m_address( boost::asio::ip::address_v4( address ) ) {
    m_name = coerce< string >( m_address.value() );
}


fostlib::host::host( uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, const nullable< string > &service )
: m_service( service.value( L"0" ) ), m_address(
    boost::asio::ip::address_v4( ( b1 << 24 ) + ( b2 << 16 ) + ( b3 << 8 ) + b4 )
) {
    m_name = coerce< string >( m_address.value() );
}


boost::asio::ip::address fostlib::host::address() const {
    if ( m_address.isnull() )
        m_address = getaddr( *this );
    return m_address.value();
}


string fostlib::host::name() const {
    return m_name;
}
string fostlib::host::service() const {
    return m_service;
}


string fostlib::coercer< string, boost::asio::ip::address >::coerce( const boost::asio::ip::address &address ) {
    return fostlib::coerce< string >( address.to_string() );
}


fostlib::exceptions::host_not_found::host_not_found( const string &hostname ) throw ()
: exception( L"Hostname: " + hostname ) {
}
const wchar_t * const fostlib::exceptions::host_not_found::message() const throw () {
    return L"Could not find an IP address for the host name";
}
