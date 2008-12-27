/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/internet/asio.hpp>


std::size_t fostlib::asio::read_until( asio::tcpsocket &sock, const std::string &term ) {
    std::size_t length( boost::asio::read_until( sock.socket, sock.data, term ) );
    sock.data.commit( length );
    return length;
}


std::string &fostlib::asio::getline( asio::tcpsocket &sock, std::string &line, const std::string &term ) {
    std::size_t length( fostlib::asio::read_until( sock, term ) );
    line = std::string();
    line.reserve( length );
    for ( std::size_t c = 0; c < length - term.size(); ++c )
        line += sock.data.sbumpc();
    for ( std::size_t c = 0; c < term.size(); ++ c )
        sock.data.sbumpc();
    return line;
}
fostlib::string &fostlib::asio::getline( asio::tcpsocket &sock, fostlib::string &line, const fostlib::string &term ) {
    std::string read;
    fostlib::asio::getline( sock, read, coerce< std::string >( term ) );
    line = coerce< string >( read );
    return line;
}


void fostlib::asio::send( asio::tcpsocket &sock, const std::string &line ) {
    boost::asio::streambuf b;
    std::ostream os( &b );
    os << line;
    std::size_t length( sock.socket.send( b.data() ) );
    b.consume( length );
}
