/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/internet/asio.hpp>


std::size_t fostlib::asio::read_until(
    boost::asio::ip::tcp::socket &sock, boost::asio::streambuf &data, const std::string &term
) {
    std::size_t length( boost::asio::read_until( sock, data, term ) );
    data.commit( length );
    return length;
}


std::string &fostlib::asio::getline( boost::asio::ip::tcp::socket &sock, std::string &line, const std::string &term ) {
    boost::asio::streambuf data;
    std::size_t length( fostlib::asio::read_until( sock, data, term ) );
    line = std::string();
    line.reserve( length );
    for ( std::size_t c = 0; c < length; ++c )
        line += data.sbumpc();
    return line;
}
fostlib::string &fostlib::asio::getline( boost::asio::ip::tcp::socket &sock, fostlib::string &line, const fostlib::string &term ) {
    std::string read;
    fostlib::asio::getline( sock, read, term.std_str() );
    line = coerce< string >( read );
    return line;
}


void fostlib::asio::send( boost::asio::ip::tcp::socket &sock, const std::string &line ) {
    boost::asio::streambuf b;
    std::ostream os( &b );
    os << string( line );
    std::size_t length( sock.send( b.data() ) );
    b.consume( length );
}
