/*
    Copyright 2001-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


namespace fostlib {


    namespace utf {


        class FOST_CORE_DECLSPEC codecvt_utf8 : public std::codecvt< utf16, utf8, utf16 > {
        public:
            codecvt_utf8( std::size_t ref );

        protected:
            bool do_always_noconv() const throw();
            int do_max_length() const throw();
            int do_encoding() const throw();

            int do_length( const state_type &, const extern_type *first1, const extern_type *last1, size_t len2 ) const throw();
            result do_in( state_type &, const extern_type *first1, const extern_type *last1, const extern_type *&mid1, intern_type *first2, intern_type *last2, intern_type *&mid2 ) const;

            result do_out( state_type &, const intern_type *first1, const intern_type *last1, const intern_type *&mid1, extern_type *first2, extern_type *last2, extern_type *&mid2 ) const;
        };


    }


    // UTF-8 file streams
    class FOST_CORE_DECLSPEC uofstream : public std::basic_ofstream< utf16 >, public boost::noncopyable {
    public:
        uofstream();
        explicit uofstream( const char *filename, std::ios_base::openmode mode = std::ios_base::out );

        void open( const char *filename, std::ios_base::openmode mode = std::ios_base::out );
        void open( const char *filename, std::ios_base::open_mode mode );

    private:
        std::locale m_locale;
    };

    class FOST_CORE_DECLSPEC uifstream : public std::basic_ifstream< utf16 >, public boost::noncopyable {
    public:
        uifstream();
        explicit uifstream( const char *filename, std::ios_base::openmode mode = std::ios_base::in );

        void open( const char *filename, std::ios_base::openmode mode = std::ios_base::in );
        void open( const char *filename, std::ios_base::open_mode mode );

        static string load( const char *filename );
        static string load( const char *filename, const string &default_content );

    private:
        std::locale m_utf8;
    };


}


fostlib::string fostlib::utf::load_file( const char *filename ) {
    return fostlib::uifstream::load( filename );
}
fostlib::string fostlib::utf::load_file( const char *filename, const fostlib::string &def ) {
    return fostlib::uifstream::load( filename, def );
}


/*
    fostlib::utf::uofstream
*/


fostlib::uofstream::uofstream()
: std::basic_ofstream< wchar_t >(), m_locale( std::locale::classic(), new utf::codecvt_utf8( 0 ) ) {
}


fostlib::uofstream::uofstream( const char *filename, std::ios_base::openmode mode )
: std::basic_ofstream< wchar_t >(), m_locale( std::locale::classic(), new utf::codecvt_utf8( 0 ) ) {
    open( filename, mode );
}


void fostlib::uofstream::open( const char *filename, std::ios_base::openmode mode ) {
    std::basic_ofstream< wchar_t >::open( filename, mode );
    imbue( m_locale );
    if ( good() ) put( wchar_t( utf::c_bom ) ); // BOM
}


void fostlib::uofstream::open( const char *filename, std::ios_base::open_mode mode ) {
    open( filename, std::ios_base::openmode( mode ) );
}


/*
    fostlib::utf::uifstream
*/


fostlib::uifstream::uifstream()
: std::basic_ifstream< utf16 >(),
        m_utf8( std::locale::classic(), new utf::codecvt_utf8( 0 ) ) {
}


fostlib::uifstream::uifstream( const char *filename, std::ios_base::openmode mode )
: std::basic_ifstream< utf16 >(),
        m_utf8( std::locale::classic(), new utf::codecvt_utf8( 0 ) ) {
    open( filename, mode );
}


void fostlib::uifstream::open( const char *filename, std::ios_base::openmode mode ) {
    std::locale &loc = m_utf8; // default to UTF8
    bool bom = false;

    std::basic_ifstream< utf8 > chk( filename, mode );
    std::ifstream::int_type u8( chk.get() );
    if ( chk.good() ) {
        if ( u8 > 0xFF )
            throw fostlib::exceptions::unicode_encoding( L"File error reading first byte" );
        else if ( u8 == 0xFF || u8 == 0x00 )
            throw fostlib::exceptions::not_implemented( L"UTF16 BE not supported" );
        else if ( u8 == 0xFE || chk.get() == 0x00 )
            throw fostlib::exceptions::not_implemented( L"UTF16 LE not supported" );
        else if ( u8 >= 0x80 && u8 <= 0xBF )
            throw fostlib::exceptions::unicode_encoding( L"File may not start with a UTF8 continuation character" );
        else if ( u8 == 0xEF && chk.get() == 0xBB && chk.get() == 0xBF )
            bom = true;
    }
    std::basic_ifstream< utf16 >::open( filename, mode );
    imbue( loc );
    if ( bom ) get(); // throw away leading BOM
}


void fostlib::uifstream::open( const char *filename, std::ios_base::open_mode mode ) {
    open( filename, std::ios_base::openmode( mode ) );
}

namespace {
    string loadfile( uifstream &file ) {
        string text;

        utf32 u32 = 0;
        size_t len = 0;
        while ( !file.eof() && file.good() ) {
            utf16 u16 = file.get();
            if ( u16 < 0x80  && len > 0 )
                throw fostlib::exceptions::unicode_encoding( L"Not enough continuation characters found" );
            else if ( u16 < 0x80 )
                text += u16;
            else if ( u16 >= 0x80 && u16 < 0xC0 ) {
                if ( len-- == 0 )
                    throw fostlib::exceptions::unicode_encoding( L"Continuation character found in wrong place" );
                u32 = ( u32 << 6 ) | ( u16 & 0x3F );
                if ( len == 0 && u32 == utf::c_bom && !text.empty() )
                    throw fostlib::exceptions::unicode_encoding( L"BOM may not appear anywhere other than at the beginning of the file" );
                else if ( len == 0 && u32 != utf::c_bom )
                    text += u32;
            } else if ( u16 >= 0xC0 && u16 < 0xE0 ) {
                len = 1;
                u32 = u16 & 0x1F;
            } else if ( u16 >= 0xE0 && u16 < 0xF0 ) {
                len = 2;
                u32 = u16 & 0x0F;
            } else if (  u16 >= 0xF0 && u16 < 0xF8 ) {
                len = 3;
                u32 = u16 & 0x07;
            } else if ( u16 == 0xFFFF && len > 0 )
                throw fostlib::exceptions::unicode_encoding( L"Not enough continuation characters found before end of file" );
        }

        return text;
    }
}
fostlib::string fostlib::uifstream::load( const char *filename ) {
    uifstream file( filename );
    string text = loadfile( file );
    if ( !file.eof() && file.bad() )
        throw fostlib::exceptions::unicode_encoding( L"Error reading from file - not all content read" );
    else
        return text;
}
fostlib::string fostlib::uifstream::load( const char *filename, const string &default_content ) {
    uifstream file( filename );
    string text = loadfile( file );
    if ( !file.eof() && file.bad() )
        return default_content;
    else
        return text;
}


/*
    fostlib::utf::codecvt_utf8
*/


fostlib::utf::codecvt_utf8::codecvt_utf8( std::size_t ref )
: std::codecvt< utf16, utf8, utf16 >( ref ) {
}


bool fostlib::utf::codecvt_utf8::do_always_noconv() const {
    return false; // shows that conversion do take place
}


int fostlib::utf::codecvt_utf8::do_max_length() const {
    return 4;
}


int fostlib::utf::codecvt_utf8::do_encoding() const {
    return 0; // the encoding involves sequences of varying lengths
}


/*
    This implementation follows the Microsoft (Dinkumware) documentation. This does not conform to the C++ standard.
    The standard states that the length is the number of extern_type and this implementation counts the number of intern_type.
*/
int fostlib::utf::codecvt_utf8::do_length( const state_type &, const extern_type *first1, const extern_type *last1, std::size_t len2 ) const {
    int count = 0;
    for ( ; first1 != last1 && std::size_t( count ) != len2; ++first1, ++count ) {
        int chars = 1;
        if ( *first1 >= 0x80 && *first1 <= 0xBF ) return count; // throw fostlib::exceptions::unicode_encoding( L"UTF-8 continuation character cannot appear without control character" );
        else if ( *first1 >= 0xC0 && *first1 < 0xE0 ) chars = 2;
        else if ( *first1 >= 0xE0 && *first1 < 0xF0 ) chars = 3;
        else if ( *first1 >= 0xF0 && *first1 < 0xF8 ) chars = 4;
        else return count; // Not a proper UTF-8 char so we stop counting already
        int check = chars;
        for ( ; check > 1 && first1 != last1; --check, ++first1 ) {
            if ( *first1 < 0x80 || *first1 > 0xBF ) return count; // throw fostlib::exceptions::unicode_encoding( L"UTF-8 continuation character is not correct" );
        }
        if ( check != 1 ) return count; // Final character was incomplete so don't count it
    }
    return count;
}


fostlib::utf::codecvt_utf8::result fostlib::utf::codecvt_utf8::do_in( state_type &, const extern_type *first1, const extern_type *last1, const extern_type *&mid1,
        intern_type *first2, intern_type *last2, intern_type *&mid2 ) const {

    utf32 ch = 0;
    std::size_t chars = ::chars_noexception( *first1 );
    if ( chars == 0 ) return error;
    else if ( first1 + chars >= last1 ) return partial;

    for ( std::size_t chk = 1; chk < chars; ++chk ) {
        if ( first1[ chk ] < 0x80 || first1[ chk ]  > 0xBF )
            return error; //throw fostlib::exceptions::unicode_encoding( L"UTF-8 continuation character is not correct (" + coerce< string >( chk ) + L" of " + coerce< string >( chars ) + L") is " + coerce< string >( seq[ chk ] ) );
    }
    switch ( chars ) {
    case 1:
        ch = utf32( first1[ 0 ] & 0x7F );
        break;
    case 2:
        ch = utf32( first1[ 0 ] & 0x1F ) << 6;
        ch |= utf32( first1[ 1 ] & 0x3F );
        break;
    case 3:
        ch = utf32( first1[ 0 ] & 0x0F ) << 12;
        ch |= utf32( first1[ 1 ] & 0x3F ) << 6;
        ch |= utf32( first1[ 2 ] & 0x3F );
        break;
    case 4:
        ch = utf32( first1[ 0 ] & 0x07 ) << 18;
        ch |= utf32( first1[ 1 ] & 0x3F ) << 12;
        ch |= utf32( first1[ 2 ] & 0x3F ) << 6;
        ch |= utf32( first1[ 3 ] & 0x3F );
        break;
    }
    if ( utf8length( ch ) != chars )
        return error; //throw fostlib::exceptions::unicode_encoding( L"UTF-8 sequence of " + coerce< string >( chars ) + L" chars generated a UTF32 character (" + coerce< string >( ch ) + L") with a different length (" + coerce< string >( utf8length( ch ) ) + L")" );

    size_t utf16len = encode( ch, first2, last2 );
    if ( utf16len == 0 ) return partial;
    else {
        mid1 = first1 + chars;
        mid2 = first2 + utf16len;
        return ok;
    }
}


fostlib::utf::codecvt_utf8::result fostlib::utf::codecvt_utf8::do_out( state_type &, const intern_type *first1, const intern_type *, const intern_type *&mid1, extern_type *first2, extern_type *last2, extern_type *&mid2 ) const {
    if ( *first1 >= 0x0000 && *first1 <= 0x007F ) {
        if ( first2 + 1 >= last2 ) return partial;
        first2[ 0 ] = static_cast< extern_type >( *first1 & 0x007F );
        mid2 = first2 + 1;
    } else if ( *first1 >= 0x0080 && *first1 <= 0x07FF ) {
        if ( first2 + 2 >= last2 ) return partial;
        first2[ 0 ] = 0xC0 | ( static_cast< extern_type >( *first1 >> 6 ) & 0x1F );
        first2[ 1 ] = 0x80 | ( static_cast< extern_type >( *first1 ) & 0x3F );
        mid2 = first2 + 2;
    } else if ( *first1 >= 0x0800 && *first1 <= 0xFFFF ) {
        if ( first2 + 3 >= last2 ) return partial;
        first2[ 0 ] = 0xE0 | ( static_cast< extern_type >( *first1 >> 12 ) & 0x0F );
        first2[ 1 ] = 0x80 | ( static_cast< extern_type >( *first1 >> 6 ) & 0x3F );
        first2[ 2 ] = 0x80 | ( static_cast< extern_type >( *first1 ) & 0x3F );
        mid2 = first2 + 3;
    } else {
        return error;
    }

    mid1 = first1 + 1;
    return ok;
}

