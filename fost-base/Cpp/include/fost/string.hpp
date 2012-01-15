/*
    Copyright 2001-2012, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_STRING_HPP
#define FOST_STRING_HPP
#pragma once


#include <fost/string-fwd.hpp>
#include <fost/rel_ops>


namespace fostlib {


    /// A Unicode aware string type
    class FOST_CORE_DECLSPEC string : public rel_ops< string > {
        native_string m_string;
    public:
        /// The base value type is a UCS4 code point
        typedef utf32 value_type;
        /// The size type
        typedef native_string::size_type size_type;
        /// The longest string
        static const size_type npos;

        /// Construct an empty string
        string();

        /// Construct a string from a UTF-8 literal up to the nil character
        string( nliteral utf8sequence );
        /// Construct a string from a UTF-8 sequence between two pointer
        string( nliteral utf8sequence_begin, nliteral utf8sequence_end );
        /// Construct a string from a UTF-16 sequence (even on platforms where wchar_t is 4 bytes)
        string( wliteral utf16sequence );
        /// Construct a string from a UTF-16 sequence between two pointers (even on platforms where wchar_t is 4 bytes)
        string( wliteral sequence_begin, wliteral sequence_end );

        /// Copy a string
        string( const string & );
        /// Copy part of a string
        string(const string &, size_type offset,
            size_type count = native_string::npos);

        /// Construct a string from a native string assuming it is in either UTF-8 or UTF-16 depending on the platform
        string( const native_string & );

        /// Construct a string of repeating code points
        string( size_type count, value_type ch );

        /// Construct a string from an iterator sequence
        template<typename I>
        string(I first, I second) {
            for ( I current = first; current != second; current++ )
                *this += *current;
        }

        /// Destruct the string
        ~string();

        /// Return the native literal version of the string
        native_literal c_str() const {
            return m_string.c_str();
        }
        /// Return the native string version of the string
        const native_string &std_str() const {
            return m_string;
        }

        /// Test for equality with a UTf-8 literal
        bool operator ==( nliteral right ) const;
        /// Test for equality with a UTF-16 literal
        bool operator ==( wliteral right ) const;
        /// Test for equality with another string
        bool operator ==( const string &right ) const;

        /// Ordering with a UTF-8 literal
        bool operator <( nliteral right ) const;
        /// Ordering with a UTF-16 literal
        bool operator <( wliteral right ) const;
        /// Ordering with another string
        bool operator <( const string &right ) const;

        /// Return a new string made from concatenating with a wide literal
        string operator +( wliteral right ) const;
        /// Concatenate the strings
        string operator +( const string &right ) const;
        /// Add a character to the string
        string operator +( value_type right ) const;

        /// Assign from a wide character literal
        string &operator =( wliteral right ) {
            return *this = string( right );
        }
        /// Assign from another string
        string &operator =( string );
        /// Assign from a single code point
        string &operator =( value_type right ) {
            return *this = string( 1, right );
        }

        string &operator +=( wliteral right );
        string &operator +=( const string &right );
        string &operator +=( value_type right );

        /// Return the code point at a certain position performing a bounds check
        utf32 at( size_type pos ) const;
        /// Return the code point at a certain position performing a bounds check
        utf32 operator[]( size_type pos ) const {
            return at( pos );
        }

        /// Return the length of the string in UCS4 code points
        size_type length() const;
        /// Return the length of the string in the underlying native character
        size_type native_length() const;
        /// Return true if the string is empty
        bool empty() const;

        /// A const iterator for walking along the string
        class FOST_CORE_DECLSPEC const_iterator
        : public std::iterator< std::forward_iterator_tag, value_type, size_type > {
            friend class fostlib::string;
            const_iterator( const native_string::const_iterator & );
        public:
            ~const_iterator() {}

            bool operator ==( const const_iterator &right ) const;
            bool operator !=( const const_iterator &right ) const {
                return !( *this == right );
            }

            utf32 operator *() const;

            const_iterator &operator++();
            const_iterator operator++( int ) {
                const_iterator it( *this );
                ++(*this);
                return it;
            }

            const_iterator operator +( size_type off );
            const_iterator &operator +=( size_type off );

        private:
            native_string::const_iterator m_it;
        };
        typedef std::reverse_iterator< const_iterator > reverse_const_iterator;

        native_literal buffer_begin() const;
        native_literal buffer_end() const;

        const_iterator begin() const;
        const_iterator offset( size_type off ) const {
            return begin() += off;
        }
        const_iterator end() const;

        /* members
        */

        void clear() {
            m_string.clear();
        }
        void swap( string &str ) {
            m_string.swap( str.m_string );
        }
        void reserve( size_type count = 0 ) {
            m_string.reserve( count );
        }

        string &erase( size_type pos = 0, size_type count = npos );

        string &insert( size_type pos, const string &str );

        size_type find( const string &str, size_type off = 0 ) const {
            if (off == npos)
                return npos;
            else return from_native( m_string.find( str.m_string, to_native( off ) ) );
        }
        size_type find( wliteral seq, size_type off = 0 ) const {
            if (off == npos)
                return npos;
            else return find( string( seq ), off );
        }
        size_type find( value_type ch, size_type off = 0 ) const {
            if (off == npos)
                return npos;
            else return find( string( 1, ch ), off );
        }

        size_type rfind( const string &str, size_type off = 0 ) const {
            return from_native( m_string.rfind( str.m_string, to_native( off ) ) );
        }
        size_type rfind( wliteral seq, size_type off = 0 ) const {
            return rfind( string( seq ), off );
        }
        size_type rfind( value_type ch, size_type off = 0 ) const {
            return rfind( string( 1, ch ), off );
        }

        size_type find_first_of( const string &str, size_type off = 0 ) const {
            return from_native( m_string.find_first_of( str.m_string, to_native( off ) ) );
        }
        size_type find_first_of( wliteral seq, size_type off = 0 ) const {
            return find_first_of( string( seq ), off );
        }
        size_type find_first_of( value_type ch, size_type off = 0 ) const {
            return find( ch, off );
        }

        size_type find_first_not_of( const string &str, size_type off = 0 ) const {
            return from_native( m_string.find_first_not_of( str.m_string, to_native( off ) ) );
        }
        size_type find_first_not_of( wliteral seq, size_type off = 0 ) const {
            return find_first_not_of( string( seq ), off );
        }
        size_type find_first_not_of( value_type ch, size_type off = 0 ) const {
            return find_first_not_of( string( 1, ch ), off );
        }

        size_type find_last_of( const string &str, size_type off = npos ) const {
            return from_native( m_string.find_last_of( str.m_string, to_native( off ) ) );
        }
        size_type find_last_of( wliteral seq, size_type off = npos ) const {
            return find_last_of( string( seq ), off );
        }
        size_type find_last_of( value_type ch, size_type off = npos ) const {
            return find_last_of( string( 1, ch ), off );
        }

        size_type find_last_not_of( const string &str, size_type off = npos ) const {
            return from_native( m_string.find_last_not_of( str.m_string, to_native( off ) ) );
        }
        size_type find_last_not_of( wliteral seq, size_type off = npos ) const {
            return find_last_not_of( string( seq ), off );
        }
        size_type find_last_not_of( value_type ch, size_type off = npos ) const {
            return find_last_not_of( string( 1, ch ), off );
        }

        string &replace( size_type off, size_type count, const string &str, size_type p2 = 0, size_type c2 = npos );
        string &replace( size_type off, size_type count, wliteral seq ) {
            return replace( off, count, string( seq ) );
        }

        string substr( size_type off = 0, size_type count = npos ) const {
            return string( *this, off, count );
        }

    private:
        size_type to_native( size_type off ) const;
        size_type from_native( size_type off ) const;
    };


    /// Allow comparison with a UTF-8 literal placed first
    inline bool operator == ( nliteral utf8_sequence, const string &str ) {
        return str == utf8_sequence;
    }
    /// Allow comparison with a UTF-16 literal placed first
    inline bool operator == ( wliteral utf16_sequence, const string &str ) {
        return str == utf16_sequence;
    }
    /// Allow comparison for inequality with UTF-8 sequence
    inline bool operator != ( nliteral utf8_sequence, const string &str ) {
        return str != utf8_sequence;
    }
    /// Allow comparison for inequality with UTF-16 sequence
    inline bool operator != ( wliteral utf16_sequence, const string &str ) {
        return str != utf16_sequence;
    }

    /// Allow ordering with a UTF-8 literal placed first
    inline bool operator < ( nliteral utf8_sequence, const string &str ) {
        return string(utf8_sequence) < str;
    }
    /// Allow ordering with a UTF-16 literal placed first
    inline bool operator < ( wliteral utf16_sequence, const string &str ) {
        return string(utf16_sequence) < str;
    }
    /// Allow ordering with a UTF-8 literal placed first
    inline bool operator > ( nliteral utf8_sequence, const string &str ) {
        return string(utf8_sequence) > str;
    }
    /// Allow ordering with a UTF-16 literal placed first
    inline bool operator > ( wliteral utf16_sequence, const string &str ) {
        return string(utf16_sequence) > str;
    }
    /// Allow ordering with a UTF-8 literal placed first
    inline bool operator <= ( nliteral utf8_sequence, const string &str ) {
        return string(utf8_sequence) <= str;
    }
    /// Allow ordering with a UTF-16 literal placed first
    inline bool operator <= ( wliteral utf16_sequence, const string &str ) {
        return string(utf16_sequence) <= str;
    }
    /// Allow ordering with a UTF-8 literal placed first
    inline bool operator >= ( nliteral utf8_sequence, const string &str ) {
        return string(utf8_sequence) >= str;
    }
    /// Allow ordering with a UTF-16 literal placed first
    inline bool operator >= ( wliteral utf16_sequence, const string &str ) {
        return string(utf16_sequence) >= str;
    }

    /// Allow expressions that add a string to the right of a character
    inline string operator +( const utf32 ch, const string &str ) {
        return string( 1, ch ) += str;
    }
    /// Allow expressions that add a string to the right of a narrow character literal
    inline string operator +( nliteral utf8Sequence, const string &str ) {
        return string( utf8Sequence ) += str;
    }
    /// Allow expressions that add a string to the right of a wide character literal
    inline string operator +( wliteral utf16Sequence, const string &str ) {
        return string( utf16Sequence ) += str;
    }


}


#ifdef FOST_COERCE_HPP
    #include <fost/string/coerce.hpp>
#endif
#ifdef FOST_ACCESSORS_HPP
    #include <fost/string/accessors.hpp>
#endif
#ifdef FOST_NULLABLE_HPP
    #include <fost/string/nullable.hpp>
#endif


namespace std {


    /// Allow the string to be output to a stream
    inline fostlib::ostream &operator <<(
        fostlib::ostream  &o, const fostlib::string &s
    ) {
        return o << s.c_str();
    }
    /// Allow the non-native string literal to be output to a stream
    inline fostlib::ostream &operator <<(
        fostlib::ostream  &o, fostlib::non_native_literal lit
    ) {
        return o << fostlib::string( lit );
    }


}


#endif // FOST_STRING_HPP

