/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_INTERNET_HTTP_USERAGENT_HPP
#define FOST_INTERNET_HTTP_USERAGENT_HPP
#pragma once


#include <fost/internet/http.hpp>


namespace fostlib {


    namespace http {


        class FOST_INET_DECLSPEC user_agent : boost::noncopyable {
        public:
            user_agent();

            class request : public http::request {
            };
            class response : public mime {
                friend class user_agent;
                response(
                    std::auto_ptr< asio::tcpsocket > socket,
                    const string &m, const url &u
                );
            public:
                accessors< const string > method;
                accessors< const url > location;

            private:
                std::auto_ptr< asio::tcpsocket > m_socket;
            };

            std::auto_ptr< response > operator () ( const string &method, const url &url, const nullable< string > &data  = null );

            std::auto_ptr< response > get( const url &url ) {
                return (*this)( L"GET", url, null );
            }
            std::auto_ptr< response > post( const url &url, const string &data ) {
                return (*this)( L"POST", url, data );
            }

        private:
            boost::asio::io_service m_service;
        };


    }


}


#endif // FOST_INTERNET_HTTP_USERAGENT_HPP
