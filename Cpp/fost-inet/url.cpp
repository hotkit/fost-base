/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-inet.hpp"
#include <fost/url.hpp>
#include <fost/exception/out_of_range.hpp>
#include <fost/exception/parse_error.hpp>
#include <fost/string/utility.hpp>
#include <fost/parse/url.hpp>


using namespace fostlib;


const fostlib::query_string_parser fostlib::query_string_p;


namespace {


    const setting< bool > g_allow_relative( L"fost-base/Cpp/fost-inet/url.cpp", L"url", L"Allow relative urls", true, true );


    wchar_t digit( utf8 dig ) {
        if ( dig < 0x0a ) return dig + L'0';
        if ( dig < 0x10 ) return dig + L'A' - 0x0a;
        throw fostlib::exceptions::out_of_range< int >( L"Number to convert to hex digit is too big", 0, 0x10, dig );
    }
    string hex( utf8 ch ) {
        wchar_t num[ 4 ];
        num[ 0 ] = '%';
        num[ 1 ] = digit( ( ch & 0xf0 ) >> 4 );
        num[ 2 ] = digit( ch & 0x0f );
        num[ 3 ] = 0;
        return string( num );
    }


    const std::string g_url_allowed( ".,:/\\_-~0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" );
    string encode( const string &str ) {
        utf8string narrowed( coerce< utf8string>( str ) );
        string encoded;
        for ( utf8string::const_iterator it( narrowed.begin() ); it != narrowed.end(); ++it )
            if ( g_url_allowed.find( *it ) == utf8string::npos )
                encoded += hex( *it );
            else
                encoded += utf32( *it );
        return encoded;
    }


    /*
     * Note that given a pathSpec ending in '/', this will leave it like that.
     * What actually happens is that a blank entry gets shoved into the list, and this recreates it later.
     * All good stuff, and exactly what we want.
     */
    string normalise_path( string t_path ) {
        while( t_path.find( L"//" ) != std::string::npos ) {
            t_path.replace( t_path.find( L"//" ), 2, L"/" );
        }
        if( t_path[0] == L'/' ) {
            t_path = t_path.substr( 1 ); // Trim off any leading / - otherwise we get a blank entry at the beginning.
        }
        std::list< string > pathlist;
        for( int i=0;; i++ ) {
            pathlist.push_back( t_path.substr(0, t_path.find_first_of(L'/')) );
            if( std::string::npos == t_path.find_first_of( L'/' ) ) {
                break;
            }
            t_path=t_path.substr( t_path.find_first_of( L'/' ) + 1 );
        }
        for( std::list< string >::iterator P=pathlist.begin(); P!=pathlist.end(); ++P ) {
            if( (*P)==L"." ) {
                pathlist.erase( P );
                P=pathlist.begin();
            } else if( (*P)==L".." ) {
                if( P==pathlist.begin() ) {
                    pathlist.erase( P );
                } else {
                    std::list< string >::iterator P1=--P;
                    ++P;
                    std::list< string >::iterator P2=++P;
                    pathlist.erase( P1, P2 );
                }
                P=pathlist.begin();
            }
        }
        t_path = L"";
        for( std::list< string >::const_iterator P=pathlist.begin(); P!=pathlist.end(); ++P ) {
            t_path += L'/' + (*P);
        }
        return replaceAll( t_path, L"//", L"/" );
    }


}


fostlib::url::query_string::query_string() {
}
fostlib::url::query_string::query_string( const string &q ) {
    split_type items( split( q, L"&" ) );
    for ( split_type::const_iterator it( items.begin() ); it != items.end(); ++it ) {
        std::pair< string, nullable< string > > parsed( partition( *it, L"=" ) );
        m_query[ parsed.first ].push_back( parsed.second );
    }
}

void fostlib::url::query_string::append( const string &name, const nullable< string > &value ) {
    if ( value.isnull() )
        m_query[ encode( name ) ].push_back( null );
    else
        m_query[ encode( name ) ].push_back( encode( value.value() ) );
}

void fostlib::url::query_string::remove( const string &name ) {
    std::map< string, std::list< nullable< string > > >::iterator p( m_query.find( name ) );
    if ( p != m_query.end() )
        m_query.erase( p );
}

nullable< string > fostlib::url::query_string::as_string() const {
    nullable< string > r;
    for ( std::map< string, std::list< nullable< string > > >::const_iterator it( m_query.begin() ); it != m_query.end(); ++it )
        for ( std::list< nullable< string > >::const_iterator v( it->second.begin() ); v != it->second.end(); ++v )
            r = concat( r, L"&", concat( it->first + L"=", *v ) );
    return r;
}


setting< string > fostlib::url::s_default_host( L"fost-base/Cpp/fost-inet/url.cpp", L"Site", L"Host", L"localhost", true );


fostlib::url::url()
: m_host( s_default_host.value(), L"http" ) {
}
fostlib::url::url( const url& url, const string &path )
: m_host( url.server() ) {
    pathspec( path );
}
fostlib::url::url( const t_form form, const string &str )
: m_host( s_default_host.value(), L"http" ) {
    std::pair< string, nullable< string > > anchor_parts( partition( str, L"#" ) );
    std::pair< string, nullable< string > > query_parts( partition( anchor_parts.first, L"?" ) );
    switch ( form ) {
    case e_pathname:
        m_pathspec = normalise_path( encode( str ) );
        break;
    case e_encoded:
        for ( string::const_iterator it( str.begin() ); it != str.end(); ++it )
            if ( *it < 0x20 || *it > 0x7f )
                throw fostlib::exceptions::parse_error( L"The encoded url contains an invalid character (" + str + L")" );
        m_pathspec = query_parts.first;
        break;
    }
    if ( !query_parts.second.isnull() )
        query( query_string( query_parts.second.value() ) );
    anchor( anchor_parts.second );
}
fostlib::url::url( const fostlib::host &h, const nullable< string > &u, const nullable< string > &pw )
: user( u ), password( pw ), m_host( h ), m_pathspec( L"/" ) {
}
fostlib::url::url( const string &a_url )
: m_host( s_default_host.value(), L"http" ) {
    string hostname;
    std::pair< string, nullable< string > > anchor_parts( partition( a_url, L"#" ) );
    std::pair< string, nullable< string > > query_parts( partition( anchor_parts.first, L"?" ) );
    std::pair< string, nullable< string > > scheme_parts( partition( query_parts.first, L":" ) );

    if ( scheme_parts.second.isnull() )
        m_pathspec = normalise_path( query_parts.first );
    else if ( scheme_parts.first == L"http" || scheme_parts.first == L"https" ) {
        m_host = fostlib::host( m_host.name(), scheme_parts.first );
        if ( scheme_parts.second.value().substr( 0, 2 ) != L"//" )
            throw exceptions::parse_error( L"Did not find // after scheme name for " + scheme_parts.first );
        std::pair< string, nullable< string > > host_parts( partition( scheme_parts.second.value().substr( 2 ), L"/" ) );
        std::pair< string, nullable< string > > user_parts( partition( host_parts.first, L"@" ) );
        if ( !user_parts.second.isnull() ) {
            std::pair< string, nullable< string > > pwd( partition( user_parts.first, L":" ) );
            if ( pwd.second.isnull() || pwd.first.empty() )
                throw exceptions::parse_error( L"Incorrect username/password format", user_parts.first );
            this->user( pwd.first );
            password( pwd.second );
            hostname = user_parts.second.value();
        } else
            hostname = user_parts.first;
        std::pair< string, nullable< string > > port( partition( hostname, L":" ) );
        m_host = fostlib::host( port.first, port.second.value() );
        m_pathspec = normalise_path( host_parts.second.value( L"/" ) );
    } else
        m_host = fostlib::host( m_host.name(), scheme_parts.first );

    if ( !query_parts.second.isnull() )
        query( query_string( query_parts.second.value() ) );
    anchor( anchor_parts.second );
}

string fostlib::url::as_string() const {
    string url( m_host.service() + L"://" );
    if ( !user().isnull() )
        url += user().value() + L":" + password().value( string() ) + L"@";
    else if ( !password().isnull() )
        url += L":" + password().value() + L"@";
    url += m_host.name();
    url += m_pathspec;
    return concat( concat( url, L"?", query().as_string() ), L"#", anchor() ).value();
}

string fostlib::url::as_string( const url &relative_from ) const {
    if ( g_allow_relative.value() &&
        ( server().service() == L"http" || server().service() == L"https" ) &&
        relative_from.server().service() == server().service() &&
        relative_from.server().name() == server().name()
    )
        return concat( concat( m_pathspec, L"?", query().as_string() ), L"#", anchor() ).value();
    else
        return as_string();
}

fostlib::host fostlib::url::server() const {
    return m_host;
}

const string &fostlib::url::pathspec() const {
    return m_pathspec;
}

void fostlib::url::pathspec( const string &a_pathName ) {
    string pathName( a_pathName );
    // First, formalise the new path.
    if( pathName.length() == 0 ) {
        pathName = L"/"; // Assume that if it's blank, the caller means '/'
    }
    // Obvious directory fixes.
    if( pathName.find( L"/." )==(pathName.length()-2) ) {
        pathName += L'/'; // Add terminating slash if it ends with /.
    } else if( pathName.find( L"/.." )==(pathName.length()-3) ) {
        pathName += L'/'; // Or /..
    } else if( pathName==L"." ) {
        pathName += L'/';
    } else if( pathName==L".." ) {
        pathName += L'/'; // Or if it's simply '..' or '.', both of which really mean '../' or './' anyway.
    }
    // Now do we add or replace?
    if ( pathName[ 0 ] != L'/' ) {
        if( m_pathspec[ m_pathspec.length() - 1 ]!=L'/' ) {
            m_pathspec+=L"/../";
        }
        // Whatever happens, we don't need the last part of the pathSpec anymore - the file name.
        m_pathspec = normalise_path( m_pathspec + pathName );
    } else { // It begins with a /, so we replace.
        m_pathspec = normalise_path( pathName );
    }
}


fostlib::exceptions::relative_path_error::relative_path_error( const string &base, const string &rel, const string &error ) throw () {
    try {
        info() << error << std::endl << L"Base comes from : " << base << std::endl << L"Relative pathname : " << rel << std::endl;
    } catch ( ... ) {
        absorbException();
    }
}
wliteral const fostlib::exceptions::relative_path_error::message() const throw () {
    return L"Relative path error";
}
