/*
    Copyright 2001-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <iostream>
#include <fost/exception/unexpected_eof.hpp>


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
                u32 = ( u8 << 6 ) | ( u8 & 0x3F );
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


string fostlib::utf::load_file( const char *filename ) {
    std::ifstream file( filename );
    string text = loadfile( file );
    if ( ( !file.eof() && file.bad() ) || text.empty() )
        throw exceptions::unexpected_eof( L"Could not load the requested file (or file empty)", string( filename ) );
    return text;
}


string fostlib::utf::load_file( const char *filename, const string &default_content ) {
    std::ifstream file( filename );
    string text = loadfile( file );
    if ( !file.eof() && file.bad() )
        return default_content;
    else
        return text;
}

