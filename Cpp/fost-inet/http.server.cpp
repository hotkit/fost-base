/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-inet.hpp"
#include <fost/internet/http.server.hpp>
#include <fost/thread.hpp>


using namespace fostlib;


fostlib::http::request::request( std::auto_ptr< asio::tcpsocket > sock )
: m_sock( sock ) {
}
namespace {
    void first_line( asio::tcpsocket &sock, nullable< std::pair< string, string > > &r ) {
        if ( r.isnull() ) {
            std::string line;
            try {
                asio::getline( sock, line, "\r\n" );
                std::pair< string, nullable< string > > parsed( partition( coerce< string >( line ), L" " ) );
                r = std::make_pair( parsed.first, partition( parsed.second.value(), L" " ).first );
            } catch ( fostlib::exceptions::exception &e ) {
                e.info() << L"Whilst reading the first line of the HTTP request" << std::endl;
                throw;
            }
        }
    }
}
const string &fostlib::http::request::method() {
    first_line( *m_sock, m_first_line );
    return m_first_line.value().first;
}
const string &fostlib::http::request::file_spec() {
    first_line( *m_sock, m_first_line );
    return m_first_line.value().second;
}

void fostlib::http::request::operator() ( const mime &response ) {
    asio::send( *m_sock, "HTTP/0.9 200 OK\r\n" );
    std::stringstream os;
    response.print_on( os );
    asio::send( *m_sock, os.str() );
}


fostlib::http::server::server( const host &h, uint16_t p )
: binding( h ), port( p ), m_server( m_service, boost::asio::ip::tcp::endpoint( binding().address(), port() ) ) {
}

std::auto_ptr< http::request > fostlib::http::server::operator() () {
    std::auto_ptr< asio::tcpsocket > sock( new asio::tcpsocket( m_service ) );
    m_server.accept( sock->socket );
    return std::auto_ptr< http::request >( new http::request( sock ) );
}
