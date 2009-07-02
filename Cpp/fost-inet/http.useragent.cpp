/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-inet.hpp"
#include <fost/internet/http.useragent.hpp>


using namespace fostlib;


/*
    fostlib::http::user_agent
*/

fostlib::http::user_agent::user_agent() {
}

std::auto_ptr< http::user_agent::response > fostlib::http::user_agent::operator () (
    const string &method, const url &url, const nullable< string > &data
) {
    std::auto_ptr< asio::tcpsocket > socket( new asio::tcpsocket( m_service ) );
    socket->socket.connect( boost::asio::ip::tcp::endpoint( url.server().address(), url.port().value( 80 ) ) );

    asio::send( *socket, coerce< utf8string >( method ) + " " + url.pathspec().underlying().underlying() + " HTTP/1.1\r\n" );

    text_body request( data.value( string() ) );
    request.headers().add( L"Host", url.server().name() );
    std::stringstream ss;
    request.print_on( ss );
    asio::send( *socket, ss.str() );

    if ( !data.isnull() )
        asio::send( *socket, coerce< utf8string>( data.value() ) );

    return std::auto_ptr< http::user_agent::response >( new http::user_agent::response( socket, method, url ) );
}


/*
    fostlib::http::user_agent::response
*/

fostlib::http::user_agent::response::response(
    std::auto_ptr< asio::tcpsocket > sock,
    const string &method, const url &url
) : method( method ), location( url ), m_socket( sock ) {
    string first_line;
    asio::getline( *m_socket, first_line, L"\r\n" );

    while ( true ) {
        string line;
        asio::getline( *m_socket, line, L"\r\n" );
        if ( line.empty() )
            break;
        headers().parse( line );
    }
    content_type( headers()[ L"Content-Type" ].value() );
}
