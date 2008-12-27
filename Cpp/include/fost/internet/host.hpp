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
#include <fost/internet/asio.hpp>


namespace fostlib {


    class FOST_INET_DECLSPEC host {
    public:
        host();
        explicit host( const string &host, const nullable< string > &service = null );
        explicit host( uint32_t ipv4, const nullable< string > &service = null );
        host( uint8_t, uint8_t, uint8_t, uint8_t, const nullable< string > &service = null  );

        boost::asio::ip::address address() const;
        string name() const;
        string service() const;

    private:
        fostlib::string m_name, m_service;
        mutable nullable< boost::asio::ip::address > m_address;
    };


    namespace exceptions {


        class FOST_INET_DECLSPEC host_not_found : public exception {
        public:
            host_not_found( const string &host_name ) throw ();

        protected:
            const wchar_t * const message() const throw ();
        };


    }

    template<>
    struct FOST_INET_DECLSPEC coercer< string, boost::asio::ip::address > {
        string coerce( const boost::asio::ip::address &i );
    };


    inline fostlib::ostream  &operator <<( fostlib::ostream  &o, const host &h ) {
        return o << h.name();
    }


}


#endif // FOST_HOST_HPP

