/**
    Copyright 1999-2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-crypto.hpp"
#include <fost/string>
#include <fost/crypto.hpp>

#include <fost/exception/not_implemented.hpp>
#include <fost/exception/out_of_range.hpp>

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include <crypto++/hmac.h>
#include <crypto++/sha.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <crypto++/md5.h>


using namespace fostlib;


string fostlib::sha1_hmac(const string &key, const string &data) {
    BOOST_STATIC_ASSERT(sizeof(std::size_t) >= sizeof(int));
    utf8_string key_utf8(coerce<utf8_string>(key)),
            data_utf8(coerce<utf8_string>(data));

    boost::array<unsigned char, CryptoPP::SHA1::DIGESTSIZE> signature;
    CryptoPP::HMAC<CryptoPP::SHA1> hmac;
    hmac.SetKey(
            reinterpret_cast<const unsigned char *>(
                    key_utf8.underlying().c_str()),
            key_utf8.underlying().length());
    hmac.Update(
            reinterpret_cast<const unsigned char *>(
                    data_utf8.underlying().c_str()),
            data_utf8.underlying().length());
    hmac.Final(signature.data());

    return coerce<string>(coerce<base64_string>(std::vector<unsigned char>(
            signature.data(), signature.data() + CryptoPP::SHA1::DIGESTSIZE)));
}


struct fostlib::hmac::impl : boost::noncopyable {
    virtual ~impl() {}

    virtual std::size_t output_size() const = 0;
    virtual void set_key(const void *key, std::size_t key_length) = 0;
    virtual void update(const unsigned char *data, std::size_t size) = 0;
    virtual void final(unsigned char *out) = 0;

    static void check(fostlib::hmac::impl *i) {
        if (!i)
            throw fostlib::exceptions::null(
                    "This hmac has not been properly initialised");
    }
};
template<typename H>
struct hmac_impl : public fostlib::hmac::impl {
    CryptoPP::HMAC<H> hmac;

    std::size_t output_size() const { return H::DIGESTSIZE; }
    void set_key(const void *key, std::size_t key_length) {
        hmac.SetKey(reinterpret_cast<const unsigned char *>(key), key_length);
    }
    void update(const unsigned char *data, std::size_t size) {
        hmac.Update(data, size);
    }
    void final(unsigned char *out) { hmac.Final(out); }
};


fostlib::hmac::hmac(digester_fn hash, f5::buffer<const f5::byte> key)
: m_implementation(nullptr) {
    if (hash == fostlib::sha1)
        m_implementation = std::make_unique<hmac_impl<CryptoPP::SHA1>>();
    else if (hash == fostlib::sha256)
        m_implementation = std::make_unique<hmac_impl<CryptoPP::SHA256>>();
    else if (hash == fostlib::md5)
        m_implementation = std::make_unique<hmac_impl<CryptoPP::Weak::MD5>>();
    else
        throw exceptions::not_implemented(
                __PRETTY_FUNCTION__,
                "-- Only sha1, sha256 and md5 are supported");
    m_implementation->set_key(key.data(), key.size());
}
fostlib::hmac::hmac(hmac &&h)
: m_implementation(std::move(h.m_implementation)) {}

fostlib::hmac::~hmac() = default;


std::vector<unsigned char> fostlib::hmac::digest() const {
    impl::check(m_implementation.get());
    impl &local(*m_implementation);

    std::unique_ptr<unsigned char> output(
            new unsigned char[local.output_size()]);
    local.final(output.get());

    return std::vector<unsigned char>(
            output.get(), output.get() + local.output_size());
}


fostlib::hmac &fostlib::hmac::operator<<(const const_memory_block &p) {
    const unsigned char *begin = reinterpret_cast<const unsigned char *>(
                                p.first),
                        *end = reinterpret_cast<const unsigned char *>(
                                p.second);
    std::size_t length = end - begin;
    if (length > std::size_t(std::numeric_limits<int>::max()))
        throw exceptions::out_of_range<uint64_t>(
                L"Message data is too long", 0, std::numeric_limits<int>::max(),
                length);
    else if (length)
        m_implementation->update(begin, length);
    return *this;
}
fostlib::hmac &fostlib::hmac::operator<<(const fostlib::utf8_string &data_utf8) {
    return (*this) << const_memory_block(
                   data_utf8.underlying().c_str(),
                   data_utf8.underlying().c_str()
                           + data_utf8.underlying().length());
}

fostlib::hmac &fostlib::hmac::operator<<(const fostlib::string &data) {
    return *this << coerce<utf8_string>(data);
}

fostlib::hmac &fostlib::hmac::
        operator<<(const boost::filesystem::path &) {
    throw fostlib::exceptions::not_implemented(
            "fostlib::hmac::operator << ( const boost::filesystem::path "
            "&filename )");
}
