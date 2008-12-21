/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_ASIO_HTTP_HPP
#define FOST_ASIO_HTTP_HPP
#pragma once


#include <fost/cli>
#include <boost/asio.hpp>


namespace fostlib {


    namespace asio {


        struct tcpsocket {
            tcpsocket( boost::asio::io_service &s )
            : socket( s ) {
            }
            boost::asio::ip::tcp::socket socket;
            boost::asio::streambuf data;
        };


        std::size_t read_until( tcpsocket &sock, const std::string &term );


        std::string &getline( tcpsocket &sock, std::string &line, const std::string &term = "\n" );
        string &getline( tcpsocket &sock, string &line, const string &term = L"\n" );


        void send( tcpsocket &sock, const std::string &line );


    }


}


#endif // FOST_ASIO_HTTP_HPP
