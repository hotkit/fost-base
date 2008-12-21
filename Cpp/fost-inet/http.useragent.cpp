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
    std::auto_ptr< boost::asio::ip::tcp::iostream > stream( new boost::asio::ip::tcp::iostream );
    stream->connect( boost::asio::ip::tcp::endpoint( url.server().address(), url.port().value( 80 ) ) );
    return std::auto_ptr< http::user_agent::response >( new http::user_agent::response( stream, method, url, data ) );
}


namespace {
    string fetch( boost::asio::ip::tcp::iostream &s ) {
        std::string r;
        std::getline( s, r, '\r' );
        char n;
        s.get( n );
        if ( n != '\n' )
            throw "Error";
        return string( r );
    }
}


/*
    fostlib::http::user_agent::response
*/

fostlib::http::user_agent::response::response(
    std::auto_ptr< boost::asio::ip::tcp::iostream > stream,
    const string &method, const url &url, const nullable< string > &data
) : method( method ), location( url ), data( data ), m_stream( stream ) {
    (*m_stream) << method << L" " << url.pathspec() << L" HTTP/1.1\r\n";

    text_body request( data.value( string() ) );
    request.headers().add( L"Host", url.server().name() );
    request.print_on( *m_stream );

    string first_line( fetch( *m_stream ) );

    while ( true ) {
        string line( fetch( *m_stream ) );
        if ( line.empty() )
            break;
        headers().parse( line );
    }
}
