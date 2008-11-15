/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_HOST_HPP
#define FOST_HOST_HPP
#pragma once


#include <fost/cli>
#include <boost/asio.hpp>


namespace fostlib {


    class FOST_INET_DECLSPEC host {
    public:
        host();
        explicit host( const fostlib::string & );
        explicit host( uint32_t ipv4 );
        host( unsigned char, unsigned char, unsigned char, unsigned char );

        boost::asio::ip::address address() const;
        fostlib::string name() const;

    private:
        fostlib::string m_name;
        mutable nullable< boost::asio::ip::address > m_address;
    };


    template<>
    struct FOST_INET_DECLSPEC coercer< string, boost::asio::ip::address > {
        string coerce( const boost::asio::ip::address &i );
    };


    inline fostlib::ostream  &operator <<( fostlib::ostream  &o, const host &h ) {
        return o << h.name();
    }


}


#endif // FOST_HOST_HPP

