/*
    Copyright 2001-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fstream>
#include <fost/detail/unicode.hpp>

#include <fost/exception/not_implemented.hpp>
#include <fost/exception/out_of_range.hpp>
#include <fost/exception/unexpected_eof.hpp>
#include <fost/exception/unicode_encoding.hpp>

#include <boost/filesystem/fstream.hpp>


using namespace fostlib;


/*
    Helper functions used to implement other parts of the Unicode handling
*/


namespace {

    std::size_t chars( const utf8 ch ) {
        if ( ch < 0x80 ) return 1;
        else if ( ch >= 0x80 && ch <= 0xBF ) throw fostlib::exceptions::unicode_encoding( L"UTF-8 continuation character (" + coerce< string >( int( ch ) ) + L") cannot appear without control character" );
        else if ( ch >= 0xC0 && ch < 0xE0 ) return 2;
        else if ( ch >= 0xE0 && ch < 0xF0 ) return 3;
        else if ( ch >= 0xF0 && ch < 0xF8 ) return 4;
        else if ( ch >= 0xF8 && ch < 0xFC ) throw fostlib::exceptions::unicode_encoding( L"UTF-8 encoding may no longer be 5 bytes long (" + coerce< string >( int( ch ) ) + L")" );
        else if ( ch >= 0xFC && ch < 0xFE ) throw fostlib::exceptions::unicode_encoding( L"UTF-8 encoding may no longer be 6 bytes long (" + coerce< string >( int( ch ) ) + L")" );
        else throw fostlib::exceptions::unicode_encoding( L"UTF-8 control character (" + coerce< string >( int( ch ) ) + L") is not recognised (could be a UTF-16 BOM)" );
    }
    std::size_t chars_noexception( const utf8 ch ) {
        if ( ch < 0x80 ) return 1;
        else if ( ch >= 0xC0 && ch < 0xE0 ) return 2;
        else if ( ch >= 0xE0 && ch < 0xF0 ) return 3;
        else if ( ch >= 0xF0 && ch < 0xF8 ) return 4;
        else return 0;
    }

}


/*
    Misc encoding & decoding functions
*/


utf32 fostlib::utf::assertValid( utf32 ch ) {
    try {
        if ( ch >= 0xD800 && ch <= 0xDBFF )
            throw fostlib::exceptions::unicode_encoding( L"UTF-32 character is in the UTF-16 leading surrogate pair range." );
        if ( ch >= 0xDC00 && ch <= 0xDFFF )
            throw fostlib::exceptions::unicode_encoding( L"UTF-32 character is in the UTF-16 trailing surrogate pair range." );
        if ( ch == 0xFFFE || ch == 0xFFFF )
            throw fostlib::exceptions::unicode_encoding( L"UTF-32 character is disallowed (0xFFFE/0xFFFF)" );
        if ( ch > 0x10FFFF )
            throw fostlib::exceptions::unicode_encoding( L"UTF-32 character is beyond the allowable range." );
        return ch;
    } catch ( fostlib::exceptions::unicode_encoding &e ) {
        e.info() << L"Character value is: " << ch << std::endl;
        throw;
    }
}


std::size_t fostlib::utf::length( nliteral seq ) {
    std::size_t count = 0;
    for ( ; *seq != 0; ++count ) {
        std::size_t chars = ::chars( *seq );
        for ( std::size_t chk = 1; chk < chars; chk++ ) {
            unsigned char current( *reinterpret_cast< const unsigned char * >( seq + chk ) );
            if ( current < 0x80 || current  > 0xBF )
                throw fostlib::exceptions::unicode_encoding( L"UTF-8 continuation character is not correct (" + coerce< string >( chk ) + L" of " + coerce< string >( chars ) + L") is " + coerce< string >( int( current ) ) );
        }
        seq += chars;
    }
    return count;
}
std::size_t fostlib::utf::length( wliteral seq ) {
    std::size_t count = 0;
    for ( ; *seq != 0; ++count ) {
        if ( *seq >= 0xD800 && *seq <= 0xDBFF ) {
            ++seq;
            if ( *seq < 0xDC00 || *seq > 0xDFFF )
                throw fostlib::exceptions::unicode_encoding( L"Trailing character in a UTF-16 surrogate pair is missing" );
            ++seq;
        } else if ( *seq >= 0xDC00 && *seq <= 0xDFFF )
            throw fostlib::exceptions::unicode_encoding( L"Trailing character in a UTF-16 surrogate pair has been found first (" + coerce< string >( int( *seq ) ) + L")" );
        else ++seq;
    }
    return count;
}
std::size_t fostlib::utf::length( wliteral seq, wliteral end ) {
    std::size_t count = 0;
    for ( ; *seq != 0 && seq != end; ++count ) {
        if ( *seq >= 0xD800 && *seq <= 0xDBFF ) {
            ++seq;
            if ( seq == end ) return count - 1;
            else if ( *seq < 0xDC00 || *seq > 0xDFFF )
                throw fostlib::exceptions::unicode_encoding( L"Trailing character in a UTF-16 surrogate pair is missing" );
            ++seq;
        } else if ( *seq >= 0xDC00 && *seq <= 0xDFFF )
            throw fostlib::exceptions::unicode_encoding( L"Trailing character in a UTF-16 surrogate pair has been found first (" + coerce< string >( int( *seq ) ) + L")" );
        else ++seq;
    }
    return count;
}


namespace {
    template< typename C >
    std::size_t character_length( utf32 ch );

    template<>
    std::size_t character_length< char >( utf32 ch ) {
        utf::assertValid( ch );
        if ( ch < 0x00080 ) return 1;
        else if ( ch < 0x00800 ) return 2;
        else if ( ch < 0x10000 ) return 3;
        else return 4;
    }

    template<>
    std::size_t character_length< utf16 >( utf32 ch ) {
        if ( ch < 0x10000 ) return 1;
        else return 2;
    }
}
std::size_t fostlib::utf::utf8length( utf32 ch ) {
    return character_length< char >( ch );
}
std::size_t fostlib::utf::utf16length( utf32 ch ) {
    return character_length< utf16 >( ch );
}
std::size_t fostlib::utf::native_length( utf32 ch ) {
    return character_length< native_char >( ch );
}


std::size_t fostlib::utf::native_length( nliteral ) {
    throw fostlib::exceptions::not_implemented( L"std::size_t fostlib::utf::native_length( const utf8 * )" );
}
std::size_t fostlib::utf::native_length( wliteral ) {
    throw fostlib::exceptions::not_implemented( L"std::size_t fostlib::utf::native_length( wliteral )" );
}


utf32 fostlib::utf::decode( nliteral seq, nliteral end ) {
    utf32 ch = 0;
    std::size_t chars = ::chars( *seq );
    if ( seq + chars > end )
        throw fostlib::exceptions::out_of_range< std::size_t >( L"There are not enough UTF-8 bytes in the sequence", chars, std::string::npos, end - seq );
    for ( std::size_t chk = 1; chk < chars; chk++ ) {
        unsigned char current( *reinterpret_cast< const unsigned char * >( seq + chk ) );
        if ( current < 0x80 || current  > 0xBF )
            throw fostlib::exceptions::unicode_encoding( L"UTF-8 continuation character is not correct (" + coerce< string >( chk ) + L" of " + coerce< string >( chars ) + L") is " + coerce< string >( int( current ) ) );
    }
    switch ( chars ) {
    case 1:
        ch = utf32( seq[ 0 ] & 0x7F );
        break;
    case 2:
        ch = utf32( seq[ 0 ] & 0x1F ) << 6;
        ch |= utf32( seq[ 1 ] & 0x3F );
        break;
    case 3:
        ch = utf32( seq[ 0 ] & 0x0F ) << 12;
        ch |= utf32( seq[ 1 ] & 0x3F ) << 6;
        ch |= utf32( seq[ 2 ] & 0x3F );
        break;
    case 4:
        ch = utf32( seq[ 0 ] & 0x07 ) << 18;
        ch |= utf32( seq[ 1 ] & 0x3F ) << 12;
        ch |= utf32( seq[ 2 ] & 0x3F ) << 6;
        ch |= utf32( seq[ 3 ] & 0x3F );
        break;
    }
    if ( utf8length( ch ) != chars )
        throw fostlib::exceptions::unicode_encoding( L"UTF-8 sequence of " + coerce< string >( chars ) + L" chars generated a UTF32 character (" + coerce< string >( int( ch ) ) + L") with a different length (" + coerce< string >( utf8length( ch ) ) + L")" );
    return ch;
}
utf32 fostlib::utf::decode( wliteral begin, wliteral end ) {
    if ( begin == 0 )
        return utf32( 0 );
    else if ( begin + 1 == end )
        return utf::decode( *begin );
    else
        return utf::decode( *begin, *(begin + 1) );
}
utf32 fostlib::utf::decode( wchar_t first ) {
    return utf::decode( first, 0 );
}
utf32 fostlib::utf::decode( wchar_t first, wchar_t second ) {
    try {
        utf32 ch = first;
        if ( ch >= 0xffff )
            throw fostlib::exceptions::unicode_encoding( L"This character is outside the allowed range for a single UTF-16 wchar_t" );
        else if ( ch >= 0xD800 && ch <= 0xDBFF ) {
            if ( second == 0 )
                throw fostlib::exceptions::unicode_encoding( L"Trailing surrogate missing from UTF-16 sequence (it is ZERO)" );
            if ( second < 0xDC00 || second > 0xDFFF )
                throw fostlib::exceptions::unicode_encoding( L"Trailing character in a UTF-16 surrogate pair is missing (outside correct range)" );
            return assertValid( ( ch << 10 ) + second + 0x10000 - ( 0xD800 << 10 ) - 0xDC00 );
        }
        return assertValid( ch );
    } catch ( fostlib::exceptions::exception &e ) {
        e.info() << L"Decoding UTF-16 number: " << coerce< string >( int( first ) ) << std::endl;
        e.info() << L"Following UTF-16 number: " << coerce< string >( int( second ) ) << std::endl;
        throw;
    }
}


std::size_t fostlib::utf::encode( utf32 ch, utf8 *begin, const utf8 *end ) {
    std::size_t sz = utf::utf8length( assertValid( ch ) );
    if ( begin + sz <= end ) {
        switch ( sz ) {
        case 1:
            begin[ 0 ] = static_cast< utf8 >( ch & 0x7F );
            break;
        case 2:
            begin[ 0 ] = 0xC0 | ( static_cast< utf8 >( ch >> 6 ) & 0x1F );
            begin[ 1 ] = 0x80 | ( static_cast< utf8 >( ch ) & 0x3F );
            break;
        case 3:
            begin[ 0 ] = 0xE0 | ( static_cast< utf8 >( ch >> 12 ) & 0x0F );
            begin[ 1 ] = 0x80 | ( static_cast< utf8 >( ch >> 6 ) & 0x3F );
            begin[ 2 ] = 0x80 | ( static_cast< utf8 >(ch) & 0x3F );
            break;
        case 4:
            begin[ 0 ] = 0xF0 | ( static_cast< utf8 >( ch >> 18 ) & 0x07 );
            begin[ 1 ] = 0x80 | ( static_cast< utf8 >( ch >> 12 ) & 0x3F );
            begin[ 2 ] = 0x80 | ( static_cast< utf8 >( ch >> 6 ) & 0x3F );
            begin[ 3 ] = 0x80 | ( static_cast< utf8 >( ch ) & 0x3F );
            break;
        default:
            throw fostlib::exceptions::out_of_range< std::size_t >( L"Number of UTF-8 bytes for a single character outside of permitted range", 1, utf::utf32_utf8_max_length, sz );
        }
        return sz;
    } else
        throw fostlib::exceptions::out_of_range< std::size_t >( L"Buffer is not long enough to hold the UTF-8 sequence for this character", sz, std::numeric_limits< std::size_t >::max(), end - begin );
}
std::size_t fostlib::utf::encode( utf32 ch, utf16 *begin, const utf16 *end ) {
    std::size_t sz = utf::utf16length( assertValid( ch ) );
    if ( begin + sz <= end ) {
        if ( sz == 1 ) begin[ 0 ] = utf16( ch );
        else {
            begin[ 0 ] = 0xD800 - ( 0x10000 >> 10 ) + static_cast< utf16 >( ch >> 10 );
            begin[ 1 ] = 0xDC00 + static_cast< utf16 >( ch & 0x3FF );
        }
        return sz;
    } else
        throw fostlib::exceptions::out_of_range< std::size_t >( L"Buffer is not long enough to hold the UTF-16 sequence for this character", sz, std::numeric_limits< std::size_t >::max(), end - begin );
}


void fostlib::utf::save_file( const boost::filesystem::wpath &filename, const string &content ) {
    boost::filesystem::ofstream file( filename );
    file << coerce< utf8string >( content );
}


namespace {
    string loadfile( std::ifstream &file ) {
        string text;

        utf32 u32 = 0;
        size_t len = 0;
        while ( !file.eof() && file.good() ) {
            int u8 = file.get();
            if ( u8 < 0x80  && len > 0 )
                throw fostlib::exceptions::unicode_encoding( L"Not enough continuation characters found" );
            else if ( u8 < 0x80 && u8 > 0 )
                text += char( u8 );
            else if ( u8 >= 0x80 && u8 < 0xC0 ) {
                if ( len-- == 0 )
                    throw fostlib::exceptions::unicode_encoding( L"Continuation character found in wrong place" );
                u32 = ( utf32( u32 ) << 6 ) | ( utf32( u8 ) & 0x3F );
                if ( len == 0 && u32 == utf::c_bom && !text.empty() )
                    throw fostlib::exceptions::unicode_encoding( L"BOM may not appear anywhere other than at the beginning of the file" );
                else if ( len == 0 && u32 != utf::c_bom )
                    text += u32;
            } else if ( u8 >= 0xC0 && u8 < 0xE0 ) {
                len = 1;
                u32 = u8 & 0x1F;
            } else if ( u8 >= 0xE0 && u8 < 0xF0 ) {
                len = 2;
                u32 = u8 & 0x0F;
            } else if (  u8 >= 0xF0 && u8 < 0xF8 ) {
                len = 3;
                u32 = u8 & 0x07;
            }
        }

        return text;
    }
}

string fostlib::utf::load_file( const boost::filesystem::wpath &filename ) {
    boost::filesystem::ifstream file( filename );
    string text = loadfile( file );
    if ( ( !file.eof() && file.bad() ) || text.empty() )
        throw exceptions::unexpected_eof( L"Could not load the requested file (or file empty)", string( filename.native_file_string().c_str() ) );
    return text;
}

string fostlib::utf::load_file( const boost::filesystem::wpath &filename, const string &default_content ) {
    boost::filesystem::ifstream file( filename );
    string text = loadfile( file );
    if ( !file.eof() && file.bad() )
        return default_content;
    else
        return text;
}
