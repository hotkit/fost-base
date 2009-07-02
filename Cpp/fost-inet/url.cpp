/*
    Copyright 1999-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-inet.hpp"
#include <fost/internet/url.hpp>
#include <fost/exception/out_of_range.hpp>
#include <fost/exception/parse_error.hpp>
#include <fost/string/utility.hpp>
#include <fost/parse/url.hpp>


using namespace fostlib;


const query_string_parser fostlib::query_string_p;
const url_parser fostlib::url_p;


namespace {


    const setting< bool > g_allow_relative( L"fost-base/Cpp/fost-inet/url.cpp", L"url", L"Allow relative urls", true, true );


    template< typename C >
    C digit( utf8 dig ) {
        if ( dig < 0x0a ) return dig + '0';
        if ( dig < 0x10 ) return dig + 'A' - 0x0a;
        throw fostlib::exceptions::out_of_range< int >( L"Number to convert to hex digit is too big", 0, 0x10, dig );
    }
    template< typename S >
    S hex( utf8 ch ) {
        typename S::value_type num[ 4 ];
        num[ 0 ] = '%';
        num[ 1 ] = digit< typename S::value_type >( ( ch & 0xf0 ) >> 4 );
        num[ 2 ] = digit< typename S::value_type >( ch & 0x0f );
        num[ 3 ] = 0;
        return S( num );
    }


    const fostlib::utf8string g_url_allowed( ".,:/\\_-~0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" );
    const fostlib::utf8string g_query_string_allowed( ".,:/\\_-~0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" );


    /*
     * Note that given a pathSpec ending in '/', this will leave it like that.
     * What actually happens is that a blank entry gets shoved into the list, and this recreates it later.
     * All good stuff, and exactly what we want.
     */
    std::string normalise_path( std::string t_path ) {
        while( t_path.find( "//" ) != std::string::npos )
            t_path.replace( t_path.find( "//" ), 2, "/" );

        if( t_path[0] == '/' )
            t_path = t_path.substr( 1 ); // Trim off any leading / - otherwise we get a blank entry at the beginning.

        typedef std::list< std::string > pathlist_type;
        pathlist_type pathlist;

        for( int i=0;; i++ ) {
            pathlist.push_back( t_path.substr(0, t_path.find_first_of('/')) );
            if( std::string::npos == t_path.find_first_of( '/' ) )
                break;
            t_path=t_path.substr( t_path.find_first_of( '/' ) + 1 );
        }
        for( pathlist_type::iterator P=pathlist.begin(); P!=pathlist.end(); ++P ) {
            if( (*P)=="." ) {
                pathlist.erase( P );
                P=pathlist.begin();
            } else if( (*P)==".." ) {
                if( P==pathlist.begin() ) {
                    pathlist.erase( P );
                } else {
                    pathlist_type::iterator P1=--P;
                    ++P;
                    pathlist_type::iterator P2=++P;
                    pathlist.erase( P1, P2 );
                }
                P=pathlist.begin();
            }
        }
        t_path = "";
        for( pathlist_type::const_iterator P=pathlist.begin(); P!=pathlist.end(); ++P ) {
            t_path += '/' + (*P);
        }
        return replaceAll( t_path, "//", "/" );
    }
    url::filepath_string normalise_path( const url::filepath_string &path ) {
        return url::filepath_string( ascii_string( normalise_path( path.underlying().underlying() ) ) );
    }


}


/*
    fostlib::url::filepath_string_tag
*/


#include <fost/exception/not_implemented.hpp>


void fostlib::url::filepath_string_tag::do_encode( fostlib::nliteral from, ascii_string &into ) {
    throw fostlib::exceptions::not_implemented( L"fostlib::url::filepath_string_tag::do_encode( fostlib::nliteral from, ascii_string &into )" );
}


void fostlib::url::filepath_string_tag::do_encode( const ascii_string &from, ascii_string &into ) {
    throw fostlib::exceptions::not_implemented( L"fostlib::url::filepath_string_tag::do_encode( const ascii_string &from, ascii_string &into )" );
}


void fostlib::url::filepath_string_tag::check_encoded( const ascii_string &s ) {
    for ( ascii_string::const_iterator c( s.begin() ); c != s.end(); ++c )
        if ( g_url_allowed.find( *c ) == utf8string::npos ) {
            if ( *c == '%' )
                for ( std::size_t p = 0; p != 2; ++p ) {
                    if ( ++c == s.end() )
                        throw fostlib::exceptions::parse_error( L"File specification escape sequence is truncated" );
                    if ( ( *c >= '0' && *c <= '9' ) || ( *c >= 'a' && *c <= 'f' ) || ( *c >= 'A' && *c <= 'F' ) )
                        ; // This is fine
                    else
                        throw fostlib::exceptions::parse_error( L"File specification contains an illegal character in an escape sequence", string( 1, *c ) );
                }
            else
                throw fostlib::exceptions::parse_error( L"File specification contains an illegal character", string( 1, *c ) );
        }
}


url::filepath_string fostlib::coercer< url::filepath_string, string >::coerce( const string &str ) {
    utf8string narrowed( fostlib::coerce< utf8string >( str ) );
    url::filepath_string encoded;
    for ( utf8string::const_iterator it( narrowed.begin() ); it != narrowed.end(); ++it )
        if ( g_url_allowed.find( *it ) == utf8string::npos )
            encoded += hex< url::filepath_string >( *it );
        else
            encoded += *it;
    return encoded;
}


/*
    fostlib::url::query_string
*/


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
    m_query[ name ].push_back( null );
}

void fostlib::url::query_string::remove( const string &name ) {
    std::map< string, std::list< nullable< string > > >::iterator p( m_query.find( name ) );
    if ( p != m_query.end() )
        m_query.erase( p );
}

namespace {
    ascii_string query_string_encode( const string &s ) {
        ascii_string r; utf8string i( coerce< utf8string >( s ) );
        for ( utf8string::const_iterator c( i.begin() ); c != i.end(); ++c )
            if ( g_query_string_allowed.find( *c ) == utf8string::npos )
                r += hex< ascii_string >( *c );
            else
                r += *c;
        return r;
    }
    nullable< ascii_string > query_string_encode( const nullable< string > &s ) {
        if ( s.isnull() )
            return null;
        else
            return query_string_encode( s.value() );
    }
}
nullable< ascii_string > fostlib::url::query_string::as_string() const {
    nullable< ascii_string > r;
    for ( std::map< string, std::list< nullable< string > > >::const_iterator it( m_query.begin() ); it != m_query.end(); ++it )
        for ( std::list< nullable< string > >::const_iterator v( it->second.begin() ); v != it->second.end(); ++v )
            r = concat( r, ascii_string( "&" ), concat( query_string_encode( it->first ) + ascii_string( "=" ), query_string_encode( *v ) ) );
    return r;
}


/*
    fostlib::url
*/


setting< string > fostlib::url::s_default_host( L"fost-base/Cpp/fost-inet/url.cpp", L"Site", L"Host", L"localhost", true );


fostlib::url::url()
: protocol( ascii_string( "http" ) ), m_host( s_default_host.value(), L"http" ), m_pathspec( "/" ) {
}
fostlib::url::url( const url& url, const filepath_string &path )
: protocol( ascii_string( "http" ) ), m_host( url.server() ), m_pathspec( "/" ) {
    pathspec( path );
}
fostlib::url::url( const t_form form, const string &str )
: protocol( ascii_string( "http" ) ), m_host( s_default_host.value(), L"http" ), m_pathspec( "/" ) {
    std::pair< string, nullable< string > > anchor_parts( partition( str, L"#" ) );
    std::pair< string, nullable< string > > query_parts( partition( anchor_parts.first, L"?" ) );
    switch ( form ) {
    case e_pathname:
        m_pathspec = url::filepath_string( ascii_string( normalise_path( coerce< url::filepath_string >( str ).underlying().underlying() ) ) );
        break;
    case e_encoded:
        for ( string::const_iterator it( str.begin() ); it != str.end(); ++it )
            if ( *it < 0x20 || *it > 0x7f )
                throw fostlib::exceptions::parse_error( L"The encoded url contains an invalid character (" + str + L")" );
        m_pathspec = coerce< url::filepath_string >( query_parts.first );
        break;
    }
    if ( !query_parts.second.isnull() )
        query( query_string( query_parts.second.value() ) );
    if ( !anchor_parts.second.isnull() )
        anchor( ascii_string( coerce< utf8string >( anchor_parts.second.value() ) ) );
}
fostlib::url::url( const fostlib::host &h, const nullable< string > &u, const nullable< string > &pw )
: protocol( ascii_string( "http" ) ), user( u ), password( pw ), m_host( h ), m_pathspec( "/" ) {
}
fostlib::url::url( const string &a_url )
: protocol( ascii_string( "http" ) ), m_host( s_default_host.value(), L"http" ), m_pathspec( "/" ) {
    url u;
    if ( !boost::spirit::parse( a_url.c_str(), url_p[ phoenix::var( u ) = phoenix::arg1 ] ).full )
        throw exceptions::parse_error( L"Could not parse URL", a_url );
    *this = u;
}
fostlib::url::url( const ascii_string &protocol, const host &h,
    const nullable< string > &username,
    const nullable< string > &password
) : protocol( protocol ), user( username ), password( password ), m_host( h ), m_pathspec( "/" ) {
}
fostlib::url::url( const ascii_string &protocol, const host &h, port_number port,
    const nullable< string > &username,
    const nullable< string > &password
) : protocol( protocol ), port( port ), user( username ), password( password ), m_host( h ), m_pathspec( "/" ) {
}

ascii_string fostlib::url::as_string() const {
    ascii_string url( protocol() + ascii_string( "://" ) );
    if ( !user().isnull() )
        url += ascii_string( coerce< utf8string >( user().value() + L":" + password().value( string() ) + L"@" ) );
    else if ( !password().isnull() )
        url += ascii_string( coerce< utf8string >( L":" + password().value() + L"@" ) );
    url += ascii_string( coerce< utf8string >( m_host.name() ) ) + pathspec().underlying();
    url = concat( url, ascii_string( "?" ), query().as_string() ).value();
    return concat( url, ascii_string( "#" ), anchor() ).value();
}

ascii_string fostlib::url::as_string( const url &relative_from ) const {
    if ( g_allow_relative.value() &&
        ( protocol() == ascii_string( "http" ) || protocol() == ascii_string( "https" ) ) &&
        relative_from.server().service() == server().service() &&
        relative_from.server().name() == server().name()
    )
        return concat( concat( m_pathspec.underlying(), ascii_string( "?" ), query().as_string() ), ascii_string( "#" ), anchor() ).value();
    else
        return as_string();
}

fostlib::host fostlib::url::server() const {
    return m_host;
}

const url::filepath_string &fostlib::url::pathspec() const {
    return m_pathspec;
}

void fostlib::url::pathspec( const url::filepath_string &a_pathName ) {
    url::filepath_string pathName( a_pathName );
    // First, formalise the new path.
    if( pathName.underlying().underlying().length() == 0 ) {
        pathName = url::filepath_string( "/" ); // Assume that if it's blank, the caller means '/'
    }
    // Obvious directory fixes.
    if( pathName.underlying().underlying().find( "/." )==(pathName.underlying().underlying().length()-2) ) {
        pathName += '/'; // Add terminating slash if it ends with /.
    } else if( pathName.underlying().underlying().find( "/.." )==(pathName.underlying().underlying().length()-3) ) {
        pathName += url::filepath_string( "/" ); // Or /..
    } else if( pathName==url::filepath_string( "." ) ) {
        pathName += url::filepath_string( "/" );
    } else if( pathName==url::filepath_string( ".." ) ) {
        pathName += url::filepath_string( "/" ); // Or if it's simply '..' or '.', both of which really mean '../' or './' anyway.
    }
    // Now do we add or replace?
    if ( pathName.underlying().underlying()[ 0 ] != '/' ) {
        if( m_pathspec.underlying().underlying()[ m_pathspec.underlying().underlying().length() - 1 ] != '/' )
            m_pathspec += url::filepath_string( "/../" );
        // Whatever happens, we don't need the last part of the pathSpec anymore - the file name.
        m_pathspec = normalise_path( m_pathspec + pathName );
    } else // It begins with a /, so we replace.
        m_pathspec = normalise_path( pathName );
}


/*
    fostlib::exceptions::relative_path_error
*/


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
