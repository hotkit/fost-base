/**
    Copyright 2009-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-crypto.hpp"
#include <fost/progress>
#include <fost/unicode>

#include <fost/crypto.hpp>

#include <fstream>
#include <fost/filesystem.hpp>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <crypto++/md5.h>
#include <crypto++/ripemd.h>
#include <crypto++/sha.h>


struct fostlib::digester::impl {
    virtual ~impl() {}

    virtual std::size_t output_size() const = 0;
    virtual void update(const unsigned char *data, std::size_t size) = 0;
    virtual void final(unsigned char *out) = 0;

    static void check(fostlib::digester::impl *i) {
        if (!i) {
            throw fostlib::exceptions::null(
                    "This digester has not been properly initialised");
        }
    }
};


template<typename H>
struct hash_impl : public fostlib::digester::impl {
    H hash;
    std::size_t output_size() const { return H::DIGESTSIZE; }
    void update(const unsigned char *data, std::size_t size) {
        hash.Update(data, size);
    }
    void final(unsigned char *out) { hash.Final(out); }
};


fostlib::digester::digester(digester_fn hash) {
    if (hash == fostlib::sha1) {
        m_implementation = std::make_unique<hash_impl<CryptoPP::SHA1>>();
    } else if (hash == fostlib::sha256) {
        m_implementation = std::make_unique<hash_impl<CryptoPP::SHA256>>();
    } else if (hash == fostlib::ripemd256) {
        m_implementation = std::make_unique<hash_impl<CryptoPP::RIPEMD256>>();
    } else if (hash == fostlib::md5) {
        m_implementation = std::make_unique<hash_impl<CryptoPP::Weak::MD5>>();
    } else {
        throw fostlib::exceptions::not_implemented(
                "fostlib::digester::digester( fostlib::string (*)( const "
                "fostlib::string & ) )"
                "with other hash functions",
                "Only sha1, sha256, ripemd256 and md5 are supported right now");
    }
}


fostlib::digester::digester(digester &&d)
: m_implementation(std::move(d.m_implementation)) {}


fostlib::digester::~digester() = default;


std::vector<unsigned char> fostlib::digester::digest() const {
    fostlib::digester::impl::check(m_implementation.get());
    impl &local(*m_implementation);

    std::unique_ptr<unsigned char> output(
            new unsigned char[local.output_size()]);
    local.final(output.get());

    return std::vector<unsigned char>(
            output.get(), output.get() + local.output_size());
}


fostlib::digester &fostlib::digester::operator<<(const const_memory_block &p) {
    fostlib::digester::impl::check(m_implementation.get());
    const unsigned char *begin = reinterpret_cast<const unsigned char *>(
                                p.first),
                        *end = reinterpret_cast<const unsigned char *>(
                                p.second);
    const std::size_t length = end - begin;
    if (length) m_implementation->update(begin, length);
    return *this;
}


fostlib::digester &fostlib::digester::operator<<(const fostlib::string &s) {
    fostlib::digester::impl::check(m_implementation.get());
    fostlib::utf8_string utf8(fostlib::coerce<fostlib::utf8_string>(s));
    m_implementation->update(
            reinterpret_cast<const unsigned char *>(utf8.memory().data()),
            utf8.memory().size());
    return *this;
}


fostlib::digester &fostlib::digester::
        operator<<(const fostlib::fs::path &filename) {
    fostlib::digester::impl::check(m_implementation.get());
    fostlib::progress progress(filename);
    fostlib::ifstream file(filename, std::ios::binary);
    while (!file.eof() && file.good()) {
        boost::array<char, 4096> buffer;
        file.read(buffer.c_array(), buffer.size());
        const std::size_t read(file.gcount());
        (*this) << const_memory_block(
                buffer.c_array(), buffer.c_array() + read);
        progress += read;
    }
    return *this;
}
