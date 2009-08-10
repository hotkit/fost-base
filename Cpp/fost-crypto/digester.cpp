/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-crypto.hpp"
#include <fost/detail/crypto.hpp>
#include <openssl/evp.h>

#include <fost/exception/not_implemented.hpp>


struct fostlib::digester::digester::impl {
    impl(const EVP_MD *type) {
        EVP_DigestInit(&mdctx, type);
    }
    impl(const impl &i) {
        EVP_DigestInit(&mdctx, EVP_MD_CTX_md(&i.mdctx));
        EVP_MD_CTX_copy_ex(&mdctx, &i.mdctx);
    }
    ~impl() {
        EVP_MD_CTX_cleanup(&mdctx);
    }

    EVP_MD_CTX mdctx;

    static void check(fostlib::digester::impl *i) {
        if ( !i ) throw fostlib::exceptions::null("This digester has not been properly initialised");
    }
};


fostlib::digester::digester( fostlib::string (*hash)( const fostlib::string & ) )
: m_implementation( NULL ) {
    if ( hash == fostlib::sha1 )
        m_implementation = new impl(EVP_sha1());
    else if ( hash == fostlib::md5 )
        m_implementation = new impl(EVP_md5());
    else
        throw fostlib::exceptions::not_implemented( "fostlib::digester::digester( fostlib::string (*)( const fostlib::string & ) ) with other hash functions", "Only sha1 and md5 are supported right now" );
}

fostlib::digester::~digester() {
    if ( m_implementation )
        delete m_implementation;
}


std::vector< unsigned char > fostlib::digester::digest() const {
    fostlib::digester::impl::check(m_implementation);
    impl local(*m_implementation);

    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len;
    EVP_DigestFinal_ex(&m_implementation->mdctx, md_value, &md_len);

    return std::vector< unsigned char >( md_value, md_value + md_len );
}

fostlib::digester &fostlib::digester::operator << ( const fostlib::string &s ) {
    fostlib::digester::impl::check(m_implementation);
    fostlib::utf8string utf8(fostlib::coerce< utf8string >( s ));
    EVP_DigestUpdate(&m_implementation->mdctx, utf8.c_str(), utf8.length());
    return *this;
}

fostlib::digester &fostlib::digester::operator << ( const boost::filesystem::wpath & ) {
    fostlib::digester::impl::check(m_implementation);
    throw fostlib::exceptions::not_implemented( "fostlib::digester::operator << ( const boost::filesystem::wpath & )" );
}
