/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_INTERNET_SMTP_HPP
#define FOST_INTERNET_SMTP_HPP
#pragma once


#include <fost/core>
#include <set>


namespace fostlib {


    struct FOST_INET_DECLSPEC rfc822_address {
        fostlib::accessors< string > email;
        fostlib::accessors< nullable< string > > name;

        rfc822_address();
        explicit rfc822_address( const string &email, const nullable< string > &name = null );
    };


}


namespace std {
    template<>
    struct less< fostlib::rfc822_address > : public std::binary_function< bool, fostlib::rfc822_address, fostlib::rfc822_address > {
        bool operator ()( const fostlib::rfc822_address &l, const fostlib::rfc822_address &r ) const {
            return std::less< fostlib::string >()( l.email(), r.email() );
        }
    };
}


#endif // FOST_INTERNET_SMTP_HPP
