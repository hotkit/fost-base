/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_INTERNET_HTTP_SERVER_HPP
#define FOST_INTERNET_HTTP_SERVER_HPP
#pragma once


#include <fost/internet/url.hpp>
#include <fost/internet/mime.hpp>


namespace fostlib {


    class http_server {
    public:
        http_server( const host &h );

        accessors< const host > binding;

    private:
        boost::asio::io_service m_service;
    };


}


#endif // FOST_INTERNET_HTTP_SERVER_HPP
