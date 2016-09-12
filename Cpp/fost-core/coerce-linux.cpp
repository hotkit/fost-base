/*
    Copyright 2002-2016, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <locale>
#include <fost/unicode.hpp>


string fostlib::coercer< string, int64_t >::coerce( int64_t t ) {
    stringstream ss;
    ss << t;
    return string( ss. str() );
}
string fostlib::coercer< string, uint64_t >::coerce( uint64_t t ) {
    stringstream ss;
    ss << t;
    return string( ss.str() );
}
string fostlib::coercer< string, double >::coerce( double f ) {
    stringstream ss;
    ss.imbue( std::locale( "C" ) );
    ss.precision( std::numeric_limits< double >::digits10 + 2 );
    ss << f;
    return string( ss.str() );
}


std::wstring fostlib::coercer<std::wstring, fostlib::utf::u8_view>::coerce(const utf::u8_view u8) {
    // TODO: Don't use this type as a proxy for utf16 strings. We can have
    // a proper tagged string now.
    std::wstring res;
    res.reserve(u8.bytes());
    utf16 u16b[2];
    for ( auto ch : u8 ) {
        utf::encode(ch, u16b, u16b + 2);
        res += u16b[0];
        if ( utf::utf16length(ch) == 2 ) {
            res += u16b[1];
        }
    }
    return res;
}
std::wstring fostlib::coercer< std::wstring, string >::coerce( const string &s ) {
    return fostlib::coerce<std::wstring>(utf::u8_view(s.c_str(), s.native_length()));
}
string fostlib::coercer< string, std::wstring >::coerce( const std::wstring &s ) {
    // TODO: This doesn't UTF16 decode properly
    string r;
    r.reserve( s.length() );
    for ( std::wstring::const_iterator p( s.begin() ); p != s.end(); ++p )
        r += *p;
    return r;
}
