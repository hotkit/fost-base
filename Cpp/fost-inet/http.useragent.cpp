/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
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

std::auto_ptr< http::user_agent::response > fostlib::http::user_agent::operator()(
    const string &method, const url &url, const nullable< string > &data
) {
    std::auto_ptr< boost::asio::ip::tcp::socket > sock( new boost::asio::ip::tcp::socket( m_service ) );
    sock->connect( boost::asio::ip::tcp::endpoint( url.server().address(), url.port().value( 80 ) ) );
    return std::auto_ptr< http::user_agent::response >( new http::user_agent::response( sock, method, url, data ) );
}


/*
    fostlib::http::user_agent::response
*/

fostlib::http::user_agent::response::response(
    std::auto_ptr< boost::asio::ip::tcp::socket > sock,
    const string &method, const url &url, const nullable< string > &data
) : method( method ), location( url ), data( data ), m_socket( sock ) {
    asio::send( *m_socket, coerce< utf8string >( method + L" /" + url.pathspec() + L" HTTP/1.1" L"\r\n" ) );
    text_body request( data.value( string() ) );
    request.headers().add( L"Host", url.server().name() );
    request.headers().add( L"User-Agent", L"Fost 4" );
    std::stringstream ss;
    request.print_on( ss );
    asio::send( *m_socket, ss.str() );

    std::string first_line;
    asio::getline( *m_socket, first_line );
    std::cout << first_line << std::endl;

    for ( std::string head; !asio::getline( *m_socket, head ).empty(); ) {
        std::cout << "*" << head << std::endl;
        headers().parse( string( head ) );
    }
}
