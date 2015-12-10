/*
    Copyright 2008-2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_CRYPTO_HPP
#define FOST_CRYPTO_HPP
#pragma once


#include <fost/core>
#include <fost/pointers>
#include <boost/filesystem.hpp>


namespace fostlib {


    extern const module c_fost_crypto;


    /// Return the requested number of cryptographically secure random bytes
    template<std::size_t N>
    std::array<unsigned char, N> crypto_bytes() {
        std::array<unsigned char, N> buffer;
        std::ifstream urandom("/dev/urandom");
        urandom.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
        return buffer;
    }


    /// Digests
    FOST_CRYPTO_DECLSPEC string md5(const string &str);
    FOST_CRYPTO_DECLSPEC string sha1(const string &str);
    FOST_CRYPTO_DECLSPEC string sha256(const string &str);


    /// Generic digester for hash algorithms.
    class FOST_CRYPTO_DECLSPEC digester : boost::noncopyable {
    public:
        /// Construct the digester from the wanted digest function
        digester(string (*digest_function)(const string &));
        ~digester();

        digester &operator << ( const const_memory_block & );
        digester &operator << ( const std::vector<unsigned char> &v ) {
            if ( v.size() ) {
                const unsigned char *begin = v.data();
                return *this << const_memory_block(begin, begin + v.size());
            } else
                return *this;
        }
        digester &operator << ( const string &str );
        digester &operator << ( const boost::filesystem::path &filename );

        std::vector< unsigned char > digest() const;

        struct impl;
    private:
        impl *m_implementation;
    };


    /// Signatures
    FOST_CRYPTO_DECLSPEC
    string sha1_hmac( const string &key, const string &data );

    class FOST_CRYPTO_DECLSPEC hmac : boost::noncopyable {
    public:
        /// Construct a HMAC with the given digest and secret
        hmac(string (*digest_function)(const string &), const string &key);
        /// Construct a HMAC with the given digest and secret
        hmac(string (*digest_function)(const string &), const void *key, std::size_t key_length);
        /// Construct a HMAC with the given digest and secret
        template<std::size_t n>
        hmac( string (*digest_function)(const string &), const std::array<unsigned char, n> &s)
        : hmac(digest_function, reinterpret_cast<const void*>(s.data()), s.size()) {
        }

        ~hmac();

        hmac &operator << ( const const_memory_block & );
        hmac &operator << ( const std::vector<unsigned char> &v ) {
            if ( v.size() ) {
                const unsigned char *begin = v.data();
                return *this << const_memory_block(begin, begin + v.size());
            } else
                return *this;
        }
        hmac &operator << ( fostlib::nliteral n ) {
            return *this << fostlib::utf8_string(n);
        }
        hmac &operator << ( const utf8_string &str );
        hmac &operator << ( const string &str );
        hmac &operator << ( const boost::filesystem::path &filename );

        std::vector< unsigned char > digest() const;

        struct impl;
    private:
        impl *m_implementation;
    };


}


#endif // FOST_CRYPTO_HPP
