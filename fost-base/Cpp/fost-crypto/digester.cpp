/*
    Copyright 2009-2012, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-crypto.hpp"
#include <fost/unicode>

#include <fost/crypto.hpp>

#include <fstream>
#include <boost/filesystem/fstream.hpp>

#ifdef __GNUC__
    #pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include <crypto++/sha.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <crypto++/md5.h>


struct fostlib::digester::impl {
    virtual ~impl() {
    }

    virtual std::size_t output_size() const = 0;
    virtual void update(const unsigned char *data, std::size_t size) = 0;
    virtual void final(unsigned char *out) = 0;

    static void check(fostlib::digester::impl *i) {
        if ( !i ) throw fostlib::exceptions::null("This digester has not been properly initialised");
    }
};
template< typename H >
struct hash_impl : public fostlib::digester::impl {
    H hash;
    std::size_t output_size() const {
        return H::DIGESTSIZE;
    }
    void update(const unsigned char *data, std::size_t size) {
        hash.Update(data, size);
    }
    void final(unsigned char *out) {
        hash.Final(out);
    }
};

fostlib::digester::digester( fostlib::string (*hash)( const fostlib::string & ) )
: m_implementation( NULL ) {
    if ( hash == fostlib::sha1 )
        m_implementation = new hash_impl<CryptoPP::SHA1>;
    else if ( hash == fostlib::md5 )
        m_implementation = new hash_impl<CryptoPP::Weak::MD5>;
    else
        throw fostlib::exceptions::not_implemented( "fostlib::digester::digester( fostlib::string (*)( const fostlib::string & ) ) with other hash functions", "Only sha1 and md5 are supported right now" );
}

fostlib::digester::~digester() {
    if ( m_implementation )
        delete m_implementation;
}


std::vector< unsigned char > fostlib::digester::digest() const {
    fostlib::digester::impl::check(m_implementation);
    impl &local(*m_implementation);

    boost::scoped_ptr< unsigned char > output(
        new unsigned char[local.output_size()]);
    local.final(output.get());

     return std::vector< unsigned char >(
         output.get(), output.get() + local.output_size());
}

fostlib::digester &fostlib::digester::operator << ( const const_memory_block &p ) {
    fostlib::digester::impl::check(m_implementation);
    const unsigned char
        *begin = reinterpret_cast< const unsigned char * >( p.first ),
        *end =  reinterpret_cast< const unsigned char * >( p.second )
    ;
    std::size_t length = end - begin;
    if ( length )
        m_implementation->update(begin, length);
    return *this;
}

fostlib::digester &fostlib::digester::operator << ( const fostlib::string &s ) {
    fostlib::digester::impl::check(m_implementation);
    fostlib::utf8_string utf8(fostlib::coerce< fostlib::utf8_string >( s ));
    m_implementation->update(
        reinterpret_cast<const unsigned char *>(utf8.underlying().c_str()),
        utf8.underlying().length());
    return *this;
}

fostlib::digester &fostlib::digester::operator << ( const boost::filesystem::wpath &filename ) {
    fostlib::digester::impl::check(m_implementation);
    boost::filesystem::ifstream file(filename, std::ios::binary);
    while ( !file.eof() && file.good() ) {
        boost::array< char, 1024 > buffer;
        file.read(buffer.c_array(), buffer.size());
        (*this) << const_memory_block(
            buffer.c_array(), buffer.c_array() + file.gcount());
    }
    return *this;
}
