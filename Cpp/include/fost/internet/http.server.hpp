/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_INTERNET_HTTP_SERVER_HPP
#define FOST_INTERNET_HTTP_SERVER_HPP
#pragma once


#include <fost/internet/http.hpp>


namespace fostlib {


    namespace http {


        class FOST_INET_DECLSPEC server : boost::noncopyable {
        public:
            explicit server( const host &h, uint16_t port = 80 );

            accessors< const host > binding;
            accessors< const uint16_t > port;

            std::auto_ptr< request > operator() ();

        private:
            boost::asio::io_service m_service;
            boost::asio::ip::tcp::acceptor m_server;
        };


    }


}


#endif // FOST_INTERNET_HTTP_SERVER_HPP
