/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/string/utility.hpp>
#include <fost/string/coerce.hpp>
#include <fost/unicode.hpp>
#include <fost/exception/unicode_encoding.hpp>

#include <iostream>


using namespace fostlib;


const fostlib::string::size_type fostlib::string::npos = native_string::npos;


/* Constructors
*/

fostlib::string::string() {
}

fostlib::string::string( nliteral pos ) {
    for ( utf32 ch = 0; *pos; pos += utf::utf8length( ch ) )
        (*this) += ( ch = utf::decode( pos, pos + utf::utf32_utf8_max_length ) );
}
fostlib::string::string( nliteral pos, nliteral end ) {
    // We would use pos != end here, but if the UTF-8 string is incorrectly formatted then we might
    // end up going past the end
    for ( utf32 ch = 0; pos < end; pos += utf::utf8length( ch ) )
        (*this) += ( ch = utf::decode( pos, end ) );
}

fostlib::string::string( wliteral pos ) {
    for ( utf32 ch = 0; *pos; pos += utf::utf16length( ch ) )
        (*this) += ( ch = utf::decode( pos, pos + utf::utf32_utf16_max_length ) );
}

fostlib::string::string( const string &str )
: m_string( str.m_string ) {
}

fostlib::string::string( const string &str, size_type o, size_type c )
: m_string( str.m_string, std::min( str.to_native( o ), str.native_length() ), str.to_native( o + c ) - str.to_native( o ) ) {
}

fostlib::string::string( const native_string &nstr )
: m_string( nstr ) {
}

fostlib::string::string( size_type count, value_type ch ) {
    size_type nlen = utf::native_length( ch );
    if ( nlen == 1 )
        m_string.append( native_string( count, ch ) );
    else {
        // We waste a bit when native_char is wchar_t, but it doesn't matter
        native_char nstr[ utf::utf32_utf8_max_length + 1 ];
        if ( utf::encode( ch, nstr, nstr + nlen ) != nlen )
            throw fostlib::exceptions::unicode_encoding( L"Encoded length of code point does not match native length" );
        nstr[ nlen ] = 0;

        m_string.reserve( nlen * count );
        for ( ; count; --count )
            m_string.append( nstr );
    }
}

fostlib::string::~string() {
}


/* Conversions
*/


bool fostlib::string::operator == ( wliteral right ) const {
    return *this == string( right );
}

bool fostlib::string::operator == ( const string &right ) const {
    return m_string == right.m_string;
}


/* Operators
*/

#include <fost/exception/not_implemented.hpp>

bool fostlib::string::operator < ( wliteral right ) const {
    return (*this) < string( right );
}

bool fostlib::string::operator < ( const string &right ) const {
    return std::less< native_string >()( m_string, right.m_string );
}

bool fostlib::string::operator > ( wliteral right ) const {
    return (*this) > string( right );
}

bool fostlib::string::operator > ( const string &right ) const {
    return m_string > right.m_string;
}


string fostlib::string::operator + ( wliteral right ) const {
    return string( *this ) += right;
}

string fostlib::string::operator + ( const string &right ) const {
    return string( *this ) += right;
}

string fostlib::string::operator +( value_type right ) const {
    return string( *this ) += right;
}


string &fostlib::string::operator = ( const std::vector< wchar_t > &sequence ) {
    utf32 ch;
    for ( std::vector< wchar_t >::const_iterator p( sequence.begin() ); p != sequence.end(); p += utf::utf16length( ch ) ) {
        if ( p + 1 == sequence.end() )
            ch = utf::decode( *p );
        else
            ch = utf::decode( *p, *( p + 1 ) );
        *this += ch;
    }
    return *this;
}
string &fostlib::string::operator = ( const std::vector< utf8 > &sequence ) {
    utf32 ch;
    for ( std::vector< utf8 >::const_iterator p( sequence.begin() ); p != sequence.end(); p += utf::utf8length( ch ) ) {
        if ( p + 1 == sequence.end() )
            ch = utf::decode( *p );
        else
            ch = utf::decode( *p, *( p + 1 ) );
        *this += ch;
    }
    return *this;
}

string &fostlib::string::operator = ( string right ) {
    swap( right );
    return *this;
}


string &fostlib::string::operator +=( wliteral right ) {
    m_string.append( string( right ).m_string );
    return *this;
}

string &fostlib::string::operator +=( const string &right ) {
    m_string.append( right.m_string );
    return *this;
}

string &fostlib::string::operator +=( value_type right ) {
    return (*this) += string( 1, right );
}


/* Accessors
*/

utf32 fostlib::string::at( size_type pos ) const {
    const_iterator p( begin() );
    p += pos;
    return *p;
}

string::size_type fostlib::string::length() const {
    string::size_type len( 0 );
    for ( string::const_iterator p( begin() ); p != end(); ++p, ++len )
        ;
    return len;
}

string::size_type fostlib::string::native_length() const {
    return m_string.length();
}

bool fostlib::string::empty() const {
    return m_string.empty();
}


/* Iterator
*/

fostlib::string::const_iterator::const_iterator( const native_string::const_iterator &it )
: m_it( it ) {
}

bool fostlib::string::const_iterator::operator == ( const const_iterator &right ) const {
    return m_it == right.m_it;
}

utf32 fostlib::string::const_iterator::operator *() const {
    return utf::decode( &*m_it, &*m_it + utf::utf32_utf8_max_length );
}

fostlib::string::const_iterator &fostlib::string::const_iterator::operator ++ () {
    m_it += utf::native_length( **this );
    return *this;
}

fostlib::string::const_iterator fostlib::string::const_iterator::operator + ( size_type off ) {
    return const_iterator( *this ) += off;
}

fostlib::string::const_iterator &fostlib::string::const_iterator::operator += ( size_type off ) {
    for ( size_type i( 0 ); i < off; ++i )
        ++( *this );
    return *this;
}


native_literal fostlib::string::buffer_begin() const {
    return m_string.c_str();
}

native_literal fostlib::string::buffer_end() const {
    return m_string.c_str() + m_string.length();
}

string::const_iterator fostlib::string::begin() const {
    return const_iterator( m_string.begin() );
}

string::const_iterator fostlib::string::end() const {
    return const_iterator( m_string.end() );
}


/* members
*/


string &fostlib::string::erase( size_type pos, size_type n ) {
    string::size_type s( to_native( pos ) );
    if ( s >= m_string.size() )
        m_string.clear();
    else
        m_string.erase( s, to_native( pos + n ) - s );
    return *this;
}


string &fostlib::string::insert( size_type pos, const string &str ) {
    m_string.insert( to_native( pos ), str.m_string );
    return *this;
}


string &fostlib::string::replace( size_type off, size_type count, const string &str, size_type p2, size_type c2 ) {
    m_string = m_string.substr( 0, to_native( off ) ) + str.substr( p2, c2 ).m_string + m_string.substr( to_native( off + count ) );
    return *this;
}


/* To/from native sizes
*/

string::size_type fostlib::string::to_native( size_type off ) const {
    if ( off == npos ) return npos;
    string::size_type nlength( 0 );
    for ( string::const_iterator p( begin() ); p != end() && off; --off, ++p )
        nlength += utf::native_length( *p );
    return nlength;
}

string::size_type fostlib::string::from_native( size_type off ) const {
    if ( off == npos ) return npos;
    string::size_type ulength( 0 );
    for ( string::const_iterator p( begin() ); p != end() && off; ++p, ++ulength )
        off -= utf::native_length( *p );
    return ulength;
}
