/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/internet/asio.hpp>


std::string &fostlib::asio::getline( boost::asio::ip::tcp::socket &sock, std::string &line, const std::string &term ) {
    boost::asio::basic_streambuf<> data;
    std::size_t length( boost::asio::read_until( sock, data, term ) );
    data.commit( length );
    std::istream is( &data );
    boost::scoped_array< char > buffer( new char[ length ] );
    is.read( buffer.get(), length );
    line = std::string();
    for ( std::size_t c = 0; c < length; ++c )
        line += buffer[ c ];
    return line;
}


void fostlib::asio::send( boost::asio::ip::tcp::socket &sock, const std::string &line ) {
    boost::asio::streambuf b;
    std::ostream os( &b );
    os << string( line );
    std::size_t length( sock.send( b.data() ) );
    b.consume( length );
}
