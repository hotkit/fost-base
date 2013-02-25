/*
    Copyright 2001-2013, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/unicode.hpp>

#include <fost/exception/unexpected_eof.hpp>
#include <fost/exception/unicode_encoding.hpp>

#include <boost/filesystem/fstream.hpp>

#if (BOOST_VERSION_MAJOR < 44)
#include <cstdio>
#endif


using namespace fostlib;


void fostlib::utf::save_file(
    const boost::filesystem::wpath &filename, const string &content
) {
    boost::filesystem::ofstream file( filename );
    file << coerce< utf8_string >( content ).underlying();
}


namespace {
    string loadfile( std::ifstream &file ) {
        string text;

        utf32 u32 = 0;
        size_t len = 0;
        while ( !file.eof() && file.good() ) {
            int u8 = file.get();
            if ( u8 < 0x80  && len > 0 )
                throw fostlib::exceptions::unicode_encoding(
                    L"Not enough continuation characters found" );
            else if ( u8 < 0x80 && u8 > 0 )
                text += char( u8 );
            else if ( u8 >= 0x80 && u8 < 0xC0 ) {
                if ( len-- == 0 )
                    throw fostlib::exceptions::unicode_encoding(
                        L"Continuation character found in wrong place" );
                u32 = ( utf32( u32 ) << 6 ) | ( utf32( u8 ) & 0x3F );
                if ( len == 0 && u32 == utf::c_bom && !text.empty() )
                    throw fostlib::exceptions::unicode_encoding(
                        L"BOM may not appear anywhere other than at the beginning of the file" );
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
        throw exceptions::unexpected_eof(
            L"Could not load the requested file (or file empty)",
            coerce<string>( filename ) );
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


boost::filesystem::wpath fostlib::unique_filename() {
#if (BOOST_VERSION_MAJOR < 44)
    return coerce<boost::filesystem::wpath>(
        string(std::tmpnam(NULL)));
#else
    return boost::filesystem::temp_directory_path() /
        boost::filesystem::unique_path();
#endif
}

