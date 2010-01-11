/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/detail/unicode.hpp>
#include <fost/detail/base64.hpp>
#include <fost/detail/hex.hpp>

#include <fost/exception/not_implemented.hpp>
#include <fost/exception/out_of_range.hpp>
#include <fost/exception/parse_error.hpp>


namespace {
    template< typename C >
    inline typename boost::enable_if<
        boost::is_signed< C >, bool
    >::type char_bound_check( C c ) {
        return c < 0;
    }
    template< typename C >
    inline typename boost::enable_if<
        boost::is_unsigned< C >, bool
    >::type char_lower_check( C c ) {
        return c > 127;
    }
    void check_range( fostlib::utf32 minimum, const std::string &s ) {
        std::size_t p = 0;
        try {
            for ( std::string::const_iterator c( s.begin() ); c != s.end(); ++c, ++p )
                if ( char_bound_check(*c) || *c < minimum )
                    throw fostlib::exceptions::out_of_range< int >(
                        L"ASCII characters outside valid range",
                        minimum, 127, *c
                    );
        } catch ( fostlib::exceptions::exception &e ) {
            e.info() << L"String up until this point: "
                << fostlib::coerce< fostlib::string >(
                    fostlib::coerce< fostlib::ascii_string >( s.substr( 0, p ) )
                )
                << L"\nChecked " << p << " characters out of " << s.size()
                << std::endl;
            throw;
        }
    }
}


/*
    fostlib::ascii_string
*/


void fostlib::ascii_string_tag::do_encode( fostlib::nliteral from, std::string &into ) {
    throw fostlib::exceptions::not_implemented( L"fostlib::ascii_string_tag::do_encode( fostlib::nliteral from, std::string &into )" );
}
void fostlib::ascii_string_tag::do_encode( const std::string &from, std::string &into ) {
    throw fostlib::exceptions::not_implemented( L"fostlib::ascii_string_tag::do_encode( const std::string &from, std::string &into )" );
}
void fostlib::ascii_string_tag::check_encoded( const std::string &s ) {
    check_range(1, s);
}

fostlib::ascii_string fostlib::coercer<
    fostlib::ascii_string, std::vector< fostlib::ascii_string::value_type >
>::coerce( const std::vector< fostlib::ascii_string::value_type > &v ) {
    return ascii_string( std::string( &v[0], v.size() ) );
}
fostlib::ascii_string fostlib::coercer< fostlib::ascii_string, fostlib::string >::coerce( const fostlib::string &s ) {
    fostlib::utf8_string utf8 = fostlib::coerce< fostlib::utf8_string >( s );
    return fostlib::ascii_string( utf8.underlying() );
}

fostlib::string fostlib::coercer< fostlib::string, fostlib::ascii_string >::coerce( const fostlib::ascii_string &s ) {
    return string(s.underlying().c_str());
}

std::wstring fostlib::coercer<
    std::wstring, fostlib::ascii_string
>::coerce( const fostlib::ascii_string &s ) {
    std::wstring r; r.resize(s.underlying().length());
    for ( std::size_t p = 0; p < s.underlying().length(); ++p )
        r[p] = s.underlying()[p];
    return r;
}


/*
    fostlib::ascii_printable_string
*/


void fostlib::ascii_printable_string_tag::do_encode( fostlib::nliteral from, std::string &into ) {
    throw fostlib::exceptions::not_implemented( L"fostlib::ascii_printable_string_tag::do_encode( fostlib::nliteral from, std::string &into )" );
}
void fostlib::ascii_printable_string_tag::do_encode( const std::string &from, std::string &into ) {
    throw fostlib::exceptions::not_implemented( L"fostlib::ascii_printable_string_tag::do_encode( const std::string &from, std::string &into )" );
}
void fostlib::ascii_printable_string_tag::check_encoded( const std::string &s ) {
    check_range(' ', s);
}

fostlib::ascii_printable_string fostlib::coercer<
    fostlib::ascii_printable_string, std::vector< fostlib::ascii_printable_string::value_type >
>::coerce( const std::vector< fostlib::ascii_printable_string::value_type > &v ) {
    return ascii_printable_string( std::string( &v[0], v.size() ) );
}
fostlib::ascii_printable_string fostlib::coercer< fostlib::ascii_printable_string, fostlib::string >::coerce( const fostlib::string &s ) {
    fostlib::utf8_string utf8 = fostlib::coerce< fostlib::utf8_string >( s );
    return fostlib::ascii_printable_string( utf8.underlying() );
}

fostlib::string fostlib::coercer< fostlib::string, fostlib::ascii_printable_string >::coerce( const fostlib::ascii_printable_string &s ) {
    return string(s.underlying().c_str());
}

std::wstring fostlib::coercer<
    std::wstring, fostlib::ascii_printable_string
>::coerce( const fostlib::ascii_printable_string &s ) {
    std::wstring r; r.resize(s.underlying().length());
    for ( std::size_t p = 0; p < s.underlying().length(); ++p )
        r[p] = s.underlying()[p];
    return r;
}
