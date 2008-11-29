/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-inet.hpp"
#include <fost/internet/mime.hpp>
#include <fost/exception/parse_error.hpp>


using namespace fostlib;


namespace {


    wliteral c_crlf = L"\r\n";


}


/*
    fostlib::headers_base
*/


fostlib::headers_base::headers_base() {
}

void fostlib::headers_base::parse( const string &headers ) {
    // This implementation ignores character encodings - assumes UTF-8 which won't work for mail headers
    for ( std::pair< string, fostlib::nullable< string > > lines( partition( headers, c_crlf ) ); !lines.first.empty(); lines = partition( lines.second, c_crlf ) ) {
        std::pair< string, fostlib::nullable< string > > line( partition( lines.first, L": " ) );
        if ( line.second.isnull() ) {
            fostlib::exceptions::parse_error exc( L"Header has no value" );
            exc.info() << line.first << std::endl;
            throw exc;
        } else
            m_headers.insert( value( line.first, line.second.value() ) );
    }
}

std::pair< fostlib::string, headers_base::content > fostlib::headers_base::value( const fostlib::string &name, const fostlib::string &value ) {
    return std::make_pair( name, content( value ) );
}

bool fostlib::headers_base::exists( const fostlib::string &n ) const {
    return m_headers.find( n ) != m_headers.end();
}
namespace {
    // Done here so it is initialised on DLL loading.
    // Should g'tee no multithreading problem as this isn't going to be used in the DLL initialisation
    const headers_base::content g_stat;
}
const headers_base::content &fostlib::headers_base::operator[]( const fostlib::string &n ) const {
    std::map< fostlib::string, content >::const_iterator p( m_headers.find( n ) );
    if ( p == m_headers.end() )
        return g_stat;
    else
        return (*p).second;
}

headers_base::content &fostlib::headers_base::add( const string &n, const content &v ) {
    return (*m_headers.insert( std::make_pair( n, v ) ).first).second;
}

fostlib::headers_base::const_iterator fostlib::headers_base::begin() const {
    return m_headers.begin();
}
fostlib::headers_base::const_iterator fostlib::headers_base::end() const {
    return m_headers.end();
}

std::ostream &fostlib::operator <<( std::ostream &o, const fostlib::headers_base &headers ) {
    for ( headers_base::const_iterator i( headers.begin() ); i != headers.end(); ++i )
        o << i->first << ": " << i->second << c_crlf;
    return o;
}


/*
    fostlib::headers_base::Value
*/


fostlib::headers_base::content::content() {
}
fostlib::headers_base::content::content( wliteral val )
: value( val ) {
}
fostlib::headers_base::content::content( const string &val )
: value( val ) {
}
fostlib::headers_base::content::content( const string &val, const std::map< string, string > &args )
: value( val ), m_subvalues( args ) {
}

headers_base::content &fostlib::headers_base::content::subvalue( const string &k, const string &v ) {
    m_subvalues[ k ] = v;
    return *this;
}

nullable< string > fostlib::headers_base::content::subvalue( const string &k ) const {
    std::map< string, string >::const_iterator p( m_subvalues.find( k ) );
    if ( p == m_subvalues.end() )
        return null;
    else
        return (*p).second;
}

headers_base::content::const_iterator fostlib::headers_base::content::begin() const {
    return m_subvalues.begin();
}
headers_base::content::const_iterator fostlib::headers_base::content::end() const {
    return m_subvalues.end();
}

std::ostream &fostlib::operator <<( std::ostream &o, const headers_base::content &value ) {
    o << value.value();
    for ( headers_base::content::const_iterator i( value.begin() ); i != value.end(); ++i )
        o << "; " << i->first << "=\"" << i->second << "\"";
    return o;
}
