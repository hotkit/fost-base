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


        std::size_t read_until( boost::asio::ip::tcp::socket &sock, boost::asio::streambuf &data, const std::string &term );


        std::string &getline( boost::asio::ip::tcp::socket &sock, std::string &line, const std::string &term = "\r\n" );
        string &getline( boost::asio::ip::tcp::socket &sock, string &line, const string &term = L"\r\n" );


        void send( boost::asio::ip::tcp::socket &sock, const std::string &line );


    }


}


#endif // FOST_ASIO_HTTP_HPP
