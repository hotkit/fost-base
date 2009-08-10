/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_CRYPTO_HPP
#define FOST_CRYPTO_HPP
#pragma once


#include <fost/core>
#include <boost/filesystem.hpp>


namespace fostlib {


    // Digests
    FOST_CRYPTO_DECLSPEC string md5( const string &str );
    FOST_CRYPTO_DECLSPEC string sha1( const string &str );

    class FOST_CRYPTO_DECLSPEC digester : boost::noncopyable {
    public:
        digester( string (*digest_function)( const string & ) );
        ~digester();

        digester &operator << ( const string &str );
        digester &operator << ( const boost::filesystem::wpath &filename );

        std::vector< unsigned char > digest() const;

    private:
        struct impl;
        impl *m_implementation;
    };


    // Signatures
    FOST_CRYPTO_DECLSPEC string sha1_hmac( const string &key, const string &data );


}


#endif // FOST_CRYPTO_HPP
