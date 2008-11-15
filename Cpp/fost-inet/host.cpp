/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-inet.hpp"
#include <fost/host.hpp>


using namespace fostlib;


namespace {



    boost::asio::ip::address addressFromName( const string &name ) {
        boost::asio::io_service io_service;
        boost::asio::ip::tcp::resolver resolver( io_service );
        boost::asio::ip::tcp::resolver::query query( coerce< std::string >( name ), "http" );
        return resolver.resolve( query )->endpoint().address();
    }


}


fostlib::host::host() {
}


fostlib::host::host( const fostlib::string &name )
: m_name( name ) {
}


fostlib::host::host( uint32_t address )
: m_address( boost::asio::ip::address_v4( address ) ) {
    m_name = coerce< string >( m_address.value() );
}


fostlib::host::host( unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4 )
: m_address( boost::asio::ip::address_v4( ( b1 << 24 ) + ( b2 << 16 ) + ( b3 << 8 ) + b4 ) ) {
    m_name = coerce< string >( m_address.value() );
}


boost::asio::ip::address fostlib::host::address() const {
    if ( m_address.isnull() )
        m_address = addressFromName( m_name );
    return m_address.value();
}


fostlib::string fostlib::host::name() const {
    return m_name;
}


string fostlib::coercer< string, boost::asio::ip::address >::coerce( const boost::asio::ip::address &address ) {
    return fostlib::coerce< string >( address.to_string() );
}
