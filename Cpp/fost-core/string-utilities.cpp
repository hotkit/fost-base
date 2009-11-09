/*
    Copyright 1999-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/detail/utility.hpp>
#include <fost/detail/coerce.hpp>


using namespace fostlib;


namespace {


    const char *c_whitespace_utf8 = " \n\r\t";
    const wchar_t *c_whitespace_utf16 = L" \n\r\t";


    template< typename S, typename C >
    S iitrim( const S &text, C seq ) {
        if ( text.empty() ) return text;
        typename S::size_type end = text.find_last_not_of( seq );
        if ( end == S::npos )
            return text.substr( text.find_first_not_of( seq ) );
        else
            return text.substr( 0, end + 1 ).substr( text.find_first_not_of( seq ) );
    }
    template< typename S, typename C >
    nullable< S > itrim( const S &text, C seq ) {
        S t = iitrim( text, seq );
        if ( t.empty() )
            return null;
        else
            return t;
    }

    template< typename S >
    std::pair< S, nullable< S > > ipartition( const S &text, const S &bound ) {
        S first;
        nullable< S > second;

        typename S::size_type start = text.underlying().find( bound.underlying() );
        if ( start == std::string::npos )
            first = trim( text ).value( S() );
        else {
            first = trim( S(text.underlying().substr( 0, start )) ).value( S() );
            second = trim( S(text.underlying().substr( start + bound.underlying().length(), std::string::npos )) );
        }

        return make_pair( first, second );
    }


}


nullable< utf8_string > fostlib::trim( const fostlib::utf8_string &text ) {
    nullable< utf8_string::impl_type > r = ::itrim( text.underlying(), c_whitespace_utf8 );
    if ( r.isnull() )
        return null;
    else
        return utf8_string( r.value() );
}
nullable< utf8_string > fostlib::trim( const nullable< fostlib::utf8_string > &text ) {
    if ( text.isnull() )
        return null;
    else
        return trim( text.value() );
}


nullable< string > fostlib::trim( const fostlib::string &text ) {
    return ::itrim( text, c_whitespace_utf16 );
}
nullable< string > fostlib::trim( const fostlib::string &text, const fostlib::string &chars ) {
    return ::itrim( text, chars );
}
nullable< string > fostlib::trim( const fostlib::nullable< fostlib::string > &text ) {
    if ( text.isnull() )
        return null;
    else
        return trim( text.value() );
}


fostlib::utf8_string fostlib::replaceAll( const fostlib::utf8_string &text, const fostlib::utf8_string &findThis, const fostlib::nullable< fostlib::utf8_string > &replaceWith ) {
    fostlib::utf8_string::impl_type ret = text.underlying();
    if ( replaceWith.isnull() ) {
        for (
            fostlib::utf8_string::impl_type::size_type p( ret.find( findThis.underlying() ) );
            p != fostlib::utf8_string::impl_type::npos;
            p = ret.find( findThis.underlying(), p )
        ) {
            ret = ret.substr( 0, p ) + ret.substr( p + findThis.underlying().length() );
        }
    } else {
        for (
            fostlib::utf8_string::impl_type::size_type p( ret.find( findThis.underlying() ) );
            p != fostlib::utf8_string::impl_type::npos;
            p = ret.find( findThis.underlying(), p + replaceWith.value().underlying().length() )
        ) {
            ret = ret.substr( 0, p ) + replaceWith.value().underlying() + ret.substr( p + findThis.underlying().length() );
        }
    }
    return ret;
}


fostlib::string fostlib::replaceAll( const fostlib::string &text, const fostlib::string &findThis, const fostlib::nullable< fostlib::string > &replaceWith ) {
    fostlib::string ret = text;
    if ( replaceWith.isnull() ) {
        for ( fostlib::string::size_type p( ret.find( findThis ) ); p != string::npos; p = ret.find( findThis, p ) )
            ret = ret.substr( 0, p ) + ret.substr( p + findThis.length() );
    } else {
        for ( fostlib::string::size_type p( ret.find( findThis ) ); p != string::npos; p = ret.find( findThis, p + replaceWith.value().length() ) )
            ret = ret.substr( 0, p ) + replaceWith.value() + ret.substr( p + findThis.length() );
    }
    return ret;
}
fostlib::nullable< fostlib::string > fostlib::replaceAll( const fostlib::nullable< fostlib::string > &text, const fostlib::string &findThis, const fostlib::nullable< fostlib::string > &replaceWith ) {
    if ( text.isnull() )
        return text;
    else
        return replaceAll( text.value(), findThis, replaceWith );
}


std::pair< fostlib::utf8_string, nullable< fostlib::utf8_string > > fostlib::partition( const fostlib::utf8_string &text, const fostlib::utf8_string &bound ) {
    return ipartition( text, bound );
}


std::pair< fostlib::utf8_string, nullable< fostlib::utf8_string > > fostlib::partition( const nullable< fostlib::utf8_string > &text, const fostlib::utf8_string &bound ) {
    if ( text.isnull() ) return std::make_pair( fostlib::utf8_string(), null );
    else return partition( text.value(), bound );
}


std::pair< fostlib::utf8_string, nullable< fostlib::utf8_string > > fostlib::partition( const fostlib::utf8_string &text ) {
    fostlib::utf8_string first;
    nullable< fostlib::utf8_string > second;

    fostlib::utf8_string::impl_type::size_type start = text.underlying().find_first_of( c_whitespace_utf8 );
    if ( start == std::string::npos )
        first = trim( text ).value( utf8_string() );
    else {
        first = trim( fostlib::utf8_string( text.underlying().substr( 0, start ) ) ).value( utf8_string() );
        second = trim( fostlib::utf8_string( text.underlying().substr( start + 1 ) ) );
        if ( second.value().empty() )
            second = null;
    }

    return std::make_pair( first, second );
}


std::pair< fostlib::utf8_string, nullable< fostlib::utf8_string > > fostlib::partition( const nullable< fostlib::utf8_string > &text ) {
    if ( text.isnull() ) return std::make_pair( fostlib::utf8_string(), null );
    else return partition( text.value() );
}


std::pair< fostlib::string, nullable< fostlib::string > > fostlib::partition( const fostlib::string &text, const fostlib::string &bound ) {
    fostlib::string first;
    nullable< fostlib::string > second;

    fostlib::string::size_type start = text.find( bound );
    if ( start == fostlib::string::npos )
        first = trim( text ).value( fostlib::string() );
    else {
        first = trim( text.substr( 0, start ) ).value( fostlib::string() );
        second = trim( text.substr( start + bound.length(), fostlib::string::npos ) );
    }

    return make_pair( first, second );
}


std::pair< fostlib::string, nullable< fostlib::string > > fostlib::partition( const nullable< fostlib::string > &text, const fostlib::string &bound ) {
    if ( text.isnull() ) return std::make_pair( fostlib::string(), null );
    else return partition( text.value(), bound );
}


std::pair< fostlib::string, nullable< fostlib::string > > fostlib::partition( const fostlib::string &text ) {
    fostlib::string first;
    nullable< fostlib::string > second;

    fostlib::string::size_type start = text.find_first_of( c_whitespace_utf16 );
    if ( start == std::wstring::npos ) {
        first = trim( text ).value( string() );
    } else {
        first = trim( text.substr( 0, start ) ).value( string() );
        second = trim( text.substr( start + 1, std::wstring::npos ) );
    }

    return std::make_pair( first, second );
}


std::pair< fostlib::string, nullable< fostlib::string > > fostlib::partition( const nullable< fostlib::string > &text ) {
    if ( text.isnull() ) return std::make_pair( fostlib::string(), null );
    else return partition( text.value() );
}


split_type fostlib::split( const fostlib::string &text, const fostlib::string &on ) {
    split_type ret;
    std::pair< fostlib::string, nullable< fostlib::string > > parts( partition( text, on ) );
    while ( !parts.second.isnull() ) {
        if ( !parts.first.empty() )
            ret.push_back( parts.first );
        parts = partition( parts.second.value(), on );
    }
    if ( !parts.first.empty() )
        ret.push_back( parts.first );
    return ret;
}


nullable< string > fostlib::concat( const nullable< string > &left, const string &mid, const nullable< string > &right ) {
    if ( left.isnull() )
        return right;
    else if ( right.isnull() )
        return left;
    else
        return left.value() + mid + right.value();
}
nullable< string > fostlib::concat( const nullable< string > &left, const nullable< string > &right ) {
    if ( left.isnull() && right.isnull() )
        return null;
    else if ( left.isnull() && !right.isnull() )
        return right;
    else if ( !left.isnull() && right.isnull() )
        return left;
    else
        return left.value() + right.value();
}


std::pair< string, nullable< string > > fostlib::crack( const string &text, const string &open, const string &close ) {
    string nut = trim( text ).value( string() );
    string::size_type spos = nut.find( open );
    if ( spos == string::npos )
        return std::make_pair( nut, null );
    else {
        string::size_type epos = nut.find( close, spos + open.length() );
        if ( epos == string::npos )
            return std::make_pair( nut, null );
        else {
            if ( open != close  ) {
                string::size_type mpos = nut.find( open, spos + open.length() );
                while ( mpos <= epos ) {
                    epos = nut.find( close, epos + close.length() );
                    mpos = nut.find( open, mpos + open.length() );
                }
            }
            return std::make_pair(
                trim( nut.substr( 0, spos ) ).value( string() ),
                trim( nut.substr( spos + open.length(), epos - spos - open.length() ) )
            );
        }
    }
}


#ifdef WIN32
    #include "string-utilities-win.cpp"
#else
    #include "string-utilities-linux.cpp"
#endif
