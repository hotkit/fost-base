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


        class user_agent : boost::noncopyable {
        public:
            user_agent();

            class request : http::request {
            };
            class response : boost::noncopyable {
                friend class user_agent;
                response( const string &m, const url &u, const nullable< string > &data );
            public:
                accessors< const string > method;
                accessors< const url > location;
                accessors< const nullable< string > > data;
                accessors< const nullable< string > > body;
            };

            std::auto_ptr< response > operator()( const string &method, const url &url, const nullable< string > &data  = null );

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
