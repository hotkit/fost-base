/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/string/utility.hpp>
#include <fost/string/coerce.hpp>


using namespace fostlib;


namespace {


    const char *c_whitespace_utf8 = " \n\r\t";
    const wchar_t *c_whitespace_utf16 = L" \n\r\t";


    template< typename S, typename C >
    S itrim( const S &text, C seq ) {
        if ( text.empty() ) return text;
        typename S::size_type end = text.find_last_not_of( seq );
        if ( end == S::npos )
            return text.substr( text.find_first_not_of( seq ) );
        else
            return text.substr( 0, end + 1 ).substr( text.find_first_not_of( seq ) );
    }


}


fostlib::utf8string fostlib::trim( const fostlib::utf8string &text ) {
    return ::itrim( text, c_whitespace_utf8 );
}
fostlib::nullable< fostlib::utf8string > fostlib::trim( const fostlib::nullable< fostlib::utf8string > &text ) {
    if ( text.isnull() )
        return null;
    else
        return trim( text.value() );
}


fostlib::string fostlib::trim( const fostlib::string &text ) {
    return ::itrim( text, c_whitespace_utf16 );
}
fostlib::string fostlib::trim( const fostlib::string &text, const fostlib::string &chars ) {
    return ::itrim( text, chars );
}
fostlib::nullable< fostlib::string > fostlib::trim( const fostlib::nullable< fostlib::string > &text ) {
    if ( text.isnull() )
        return null;
    else
        return trim( text.value() );
}


fostlib::utf8string fostlib::replaceAll( const fostlib::utf8string &text, const fostlib::utf8string &findThis, const fostlib::nullable< fostlib::utf8string > &replaceWith ) {
    fostlib::utf8string ret = text;
    if ( replaceWith.isnull() ) {
        for ( fostlib::utf8string::size_type p( ret.find( findThis ) ); p != std::string::npos; p = ret.find( findThis, p ) ) {
            ret = ret.substr( 0, p ) + ret.substr( p + findThis.length() );
        }
    } else {
        for ( fostlib::utf8string::size_type p( ret.find( findThis ) ); p != std::string::npos; p = ret.find( findThis, p + replaceWith.value().length() ) ) {
            ret = ret.substr( 0, p ) + replaceWith.value() + ret.substr( p + findThis.length() );
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


std::pair< fostlib::utf8string, nullable< fostlib::utf8string > > fostlib::partition( const fostlib::utf8string &text, const fostlib::utf8string &bound ) {
    fostlib::utf8string first;
    nullable< fostlib::utf8string > second;

    fostlib::utf8string::size_type start = text.find( bound );
    if ( start == std::string::npos ) {
        first = trim( text );
    } else {
        first = trim( text.substr( 0, start ) );
        second = trim( text.substr( start + bound.size(), std::wstring::npos ) );
    }

    return make_pair( first, second );
}


std::pair< fostlib::utf8string, nullable< fostlib::utf8string > > fostlib::partition( const nullable< fostlib::utf8string > &text, const fostlib::utf8string &bound ) {
    if ( text.isnull() ) return std::make_pair( fostlib::utf8string(), null );
    else return partition( text.value(), bound );
}


std::pair< fostlib::utf8string, nullable< fostlib::utf8string > > fostlib::partition( const fostlib::utf8string &text ) {
    fostlib::utf8string first;
    nullable< fostlib::utf8string > second;

    fostlib::utf8string::size_type start = text.find_first_of( c_whitespace_utf8 );
    if ( start == std::string::npos ) {
        first = trim( text );
    } else {
        first = trim( text.substr( 0, start ) );
        second = trim( text.substr( start + 1, std::string::npos ) );
    }

    return std::make_pair( first, second );
}


std::pair< fostlib::utf8string, nullable< fostlib::utf8string > > fostlib::partition( const nullable< fostlib::utf8string > &text ) {
    if ( text.isnull() ) return std::make_pair( fostlib::utf8string(), null );
    else return partition( text.value() );
}


std::pair< fostlib::string, nullable< fostlib::string > > fostlib::partition( const fostlib::string &text, const fostlib::string &bound ) {
    fostlib::string first;
    nullable< fostlib::string > second;

    fostlib::string::size_type start = text.find( bound );
    if ( start == string::npos )
        first = trim( text );
    else {
        first = trim( text.substr( 0, start ) );
        second = trim( text.substr( start + bound.length(), string::npos ) );
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
        first = trim( text );
    } else {
        first = trim( text.substr( 0, start ) );
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
    if ( left.isnull() && right.isnull() )
        return null;
    else if ( left.isnull() && !right.isnull() )
        return right;
    else if ( !left.isnull() && right.isnull() )
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


#ifdef WIN32
    #include "string-utilities-win.cpp"
#else
    #include "string-utilities-linux.cpp"
#endif
