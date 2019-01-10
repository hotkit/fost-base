/**
    Copyright 2008-2019, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#ifndef FOST_CRYPTO_HPP
#define FOST_CRYPTO_HPP
#pragma once


#include <fost/array>
#include <fost/pointers>
#include <boost/filesystem.hpp>

// TODO Older libc6-dev packages don't provide this header :(
// This needs to be fixed using C++17's `__has_include`
// #include <sys/random.h>


namespace fostlib {


    extern const module c_fost_crypto;


    /// Constant time comparison of two memory buffers
    bool crypto_compare(
            array_view<const unsigned char> left,
            array_view<const unsigned char> right);
    /// We want to be able to do this with std::string instances
    inline bool
            crypto_compare(const std::string &left, const std::string &right) {
        return crypto_compare(
                array_view<const unsigned char>(
                        reinterpret_cast<const unsigned char *>(left.c_str()),
                        left.length()),
                array_view<const unsigned char>(
                        reinterpret_cast<const unsigned char *>(right.c_str()),
                        right.length()));
    }
    /// Allow comparison of strings
    inline bool crypto_compare(f5::u8view left, f5::u8view right) {
        return crypto_compare(
                array_view<const unsigned char>(left),
                array_view<const unsigned char>(right));
    }
    /// Allow us to compare tagged string
    template<typename T, typename U>
    inline bool crypto_compare(
            const tagged_string<T, U> &left, const tagged_string<T, U> &right) {
        return crypto_compare(
                static_cast<f5::u8view>(left), static_cast<f5::u8view>(right));
    }


    /// Return the requested number of cryptographically secure random bytes
    template<std::size_t N>
    std::array<f5::byte, N> crypto_bytes() {
        std::array<f5::byte, N> buffer;
        std::ifstream urandom("/dev/urandom");
        urandom.read(reinterpret_cast<char *>(buffer.data()), buffer.size());
        // TODO Until we can check for the presence of the sys/random.h
        // header we have to use the slower implementation
        // for ( auto n = N; n > 0; n = n - getrandom(buffer.data() + (N - n),
        // n, 0) );
        return buffer;
    }


    /// ## Cryptographic hashing functions
    FOST_CRYPTO_DECLSPEC string md5(const f5::u8view &);
    FOST_CRYPTO_DECLSPEC string sha1(const f5::u8view &);
    FOST_CRYPTO_DECLSPEC string sha256(const f5::u8view &);
    FOST_CRYPTO_DECLSPEC string ripemd256(const f5::u8view &);

    /// The type of a digester used as an argument
    using digester_fn = string (*)(const f5::u8view &);


    /// Generic digester for hash algorithms.
    class FOST_CRYPTO_DECLSPEC digester {
      public:
        /// Construct the digester from the wanted digest function
        digester(digester_fn);
        /// Make movable
        digester(digester &&);
        ~digester();

        digester &operator<<(const const_memory_block &);
        digester &operator<<(const std::vector<unsigned char> &v) {
            if (v.size()) {
                const unsigned char *begin = v.data();
                return *this << const_memory_block(begin, begin + v.size());
            } else
                return *this;
        }
        digester &operator<<(const string &str);
        digester &operator<<(const boost::filesystem::path &filename);

        std::vector<unsigned char> digest() const;

        struct impl;

      private:
        std::unique_ptr<impl> m_implementation;
    };


    /// Signatures
    FOST_CRYPTO_DECLSPEC
    string sha1_hmac(const string &key, const string &data);

    class FOST_CRYPTO_DECLSPEC hmac {
      public:
        /// Construct a HMAC with the given digest and secret
        hmac(digester_fn, f5::buffer<const f5::byte>);
        hmac(digester_fn d, const string &key)
        : hmac(d, f5::buffer<const f5::byte>{f5::u8view{key}}) {}
        hmac(digester_fn d, const void *key, std::size_t key_length)
        : hmac(d,
               f5::buffer<const f5::byte>(
                       reinterpret_cast<unsigned char const *>(key),
                       key_length)) {}
        template<std::size_t N>
        hmac(digester_fn digest_function, const std::array<unsigned char, N> &s)
        : hmac(digest_function,
               reinterpret_cast<const void *>(s.data()),
               s.size()) {}
        /// Make movable
        hmac(hmac &&);
        ~hmac();

        hmac &operator<<(const const_memory_block &);
        hmac &operator<<(const std::vector<unsigned char> &v) {
            if (v.size()) {
                const unsigned char *begin = v.data();
                return *this << const_memory_block(begin, begin + v.size());
            } else
                return *this;
        }
        hmac &operator<<(fostlib::nliteral n) {
            return *this << fostlib::utf8_string(n);
        }
        hmac &operator<<(f5::u8view);
        hmac &operator<<(const boost::filesystem::path &filename);

        std::vector<unsigned char> digest() const;

        struct impl;

      private:
        std::unique_ptr<impl> m_implementation;
    };


}


#endif // FOST_CRYPTO_HPP
