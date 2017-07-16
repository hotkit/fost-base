/*
    Copyright 2001-2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_UNICODE_HPP
#define FOST_UNICODE_HPP
#pragma once


#include <f5/cord/unicode.hpp>
#include <fost/array>
#include <fost/file.hpp>
#include <fost/tagged-string.hpp>
#include <fost/exception/unicode_encoding.hpp>


namespace fostlib {


    namespace utf {


        const utf32 c_bom = 0xFEFF;
        const std::size_t utf32_utf8_max_length = 4;
        const std::size_t utf32_utf16_max_length = 2;

        // Checks that a given utf32 character is valid - throws an exception if it isn't
        inline utf32 assertValid(utf32 codepoint) {
            try {
                f5::cord::check_valid<fostlib::exceptions::unicode_encoding>(codepoint);
                return codepoint;
            } catch ( fostlib::exceptions::unicode_encoding &e ) {
                const static jcursor pos("code-point", "utf-32");
                pos.insert(e.data(), codepoint);
                throw;
            }
        }

        // Returns the number utf32 code points in the string
        FOST_CORE_DECLSPEC std::size_t length( nliteral );
        FOST_CORE_DECLSPEC std::size_t length( nliteral, nliteral );
        FOST_CORE_DECLSPEC std::size_t length( wliteral );
        FOST_CORE_DECLSPEC std::size_t length( wliteral, wliteral );

        // Return how many utf8/utf16 characters are needed for this code point or string
        inline std::size_t utf8length(utf32 codepoint) {
            try {
                return f5::cord::u8length<fostlib::exceptions::unicode_encoding>(codepoint);
            } catch ( fostlib::exceptions::unicode_encoding &e ) {
                const static jcursor pos("code-point", "utf-32");
                pos.insert(e.data(), codepoint);
                throw;
            }
        }
        FOST_CORE_DECLSPEC std::size_t utf16length( utf32 codepoint );
        FOST_CORE_DECLSPEC std::size_t native_length( utf32 codepoint );

        // The number of native characters to encode the length
        FOST_CORE_DECLSPEC std::size_t native_length( nliteral );
        FOST_CORE_DECLSPEC std::size_t native_length( wliteral );

        // Fetch the next utf32 character from the sequence (or throw an exception)
        FOST_CORE_DECLSPEC utf32 decode( nliteral begin, nliteral end );
        FOST_CORE_DECLSPEC utf32 decode( wliteral begin, wliteral end );
        FOST_CORE_DECLSPEC utf32 decode( wchar_t first, wchar_t second );
        FOST_CORE_DECLSPEC utf32 decode( wchar_t first );

        /// Encode the character into the stream. Return the number of
        /// utf8/utf16 characters written (or throw if the buffer isn't long enough)
        inline std::size_t encode(utf32 ch, utf8 *begin, const utf8 *end) {
            try {
                const auto bytes = f5::u8encode<fostlib::exceptions::unicode_encoding>(ch);
                if ( begin + bytes.first <= end ) {
                    for ( auto b = 0; b != bytes.first; ++b, ++begin ) {
                        *begin = bytes.second[b];
                    }
                    return bytes.first;
                } else {
                    throw fostlib::exceptions::out_of_range<std::size_t>(
                        "Buffer is not long enough to hold the UTF-8 sequence for this character",
                        bytes.first, std::numeric_limits< std::size_t >::max(), end - begin);
                }
            } catch ( fostlib::exceptions::unicode_encoding &e ) {
                const static jcursor pos("code-point", "utf-32");
                pos.insert(e.data(), ch);
                throw;
            }
        }
        inline std::size_t encode(utf32 codepoint, char *begin, char *end) {
            return encode( codepoint, reinterpret_cast< utf8 * >( begin ), reinterpret_cast< utf8 * >( end ) );
        }
        FOST_CORE_DECLSPEC std::size_t encode( utf32 codepoint, utf16 *begin, const utf16 *end );


        /// For unsigned char types with an UTF-8 encoding
        class u8_view {
            using buffer_type = array_view<const unsigned char>;
            buffer_type buffer;
        public:
            u8_view() {}

            u8_view(buffer_type b)
            : buffer(b) {
            }

            template<std::size_t N>
            u8_view(const char (&s)[N])
            : buffer(reinterpret_cast<const unsigned char *>(s), N-1) {
            }

            u8_view(nliteral b, std::size_t s)
            : buffer(reinterpret_cast<const unsigned char *>(b), s) {
            }

            u8_view(const utf8_string &u8)
            : buffer(
                    reinterpret_cast<const unsigned char *>(u8.underlying().data()),
                    u8.underlying().size())
            {
            }

            explicit u8_view(const std::string &u8)
            : buffer(
                    reinterpret_cast<const unsigned char *>(u8.data()),
                    u8.size())
            {
            }

            /// An iterator that spits out UTF32 code points from the string
            class const_iterator : public std::iterator<
                    std::forward_iterator_tag,
                    utf32,
                    std::ptrdiff_t,
                    const utf32 *,
                    utf32>
            {
                friend class u8_view;
                buffer_type buffer;

                const_iterator(buffer_type b)
                : buffer(b) {
                }
            public:
                const_iterator() {}

                utf32 operator * () const {
                    return decode(reinterpret_cast<nliteral>(buffer.begin()),
                        reinterpret_cast<nliteral>(buffer.end()));
                }
                const_iterator &operator ++ () {
                    const auto here = **this;
                    const auto bytes = utf8length(here);
                    buffer = buffer_type(buffer.data() + bytes, buffer.size() - bytes);
                    return *this;
                }
                const_iterator operator ++ (int) {
                    const_iterator ret{*this};
                    ++(*this);
                    return ret;
                }

                const_iterator &operator += (std::size_t cps) {
                    while ( cps-- ) // Not undefined behaviour
                        ++(*this);
                    return *this;
                }

                bool operator == (const_iterator it) const {
                    return buffer.data() == it.buffer.data();
                }
                bool operator != (const_iterator it) const {
                    return buffer.data() != it.buffer.data();
                }
            };

            /// An iterator that produces UTF16 code points from the string
            using const_u16_iterator = f5::const_u32u16_iterator<const_iterator>;

            /// Construct a u8_view from part of another
            u8_view(const_iterator s, const_iterator e)
            : buffer(s.buffer.data(), s.buffer.size() - e.buffer.size()) {
            }

            /// Return the data array
            const char *data() const {
                return reinterpret_cast<const char *>(buffer.data());
            }

            /// Return the size in bytes of the string
            std::size_t bytes() const {
                return buffer.size();
            }

            /// Return the begin iterator that delivers UTF32 code points
            const_iterator begin() const {
                return buffer;
            }
            /// Return the end iterator that delivers UTF32 code points
            const_iterator end() const {
                return buffer_type(buffer.data() + buffer.size(), 0u);
            }

            /// Return the begin iterator that delivers UTF16 code points
            const_u16_iterator u16begin() const {
                return const_u16_iterator(begin(), end());
            }
            /// Return the end iterator that delivers UTF16 code points
            const_u16_iterator u16end() const {
                return const_u16_iterator(end(), end());
            }
        };


    }


    /// Allow us to coerce a UTF8 sequence to a UTF16 std::wstring
    template<>
    struct FOST_CORE_DECLSPEC coercer<std::wstring, utf::u8_view> {
        std::wstring coerce(utf::u8_view);
    };
    /// Turn a u8_view into JSON
    template<>
    struct coercer<json, utf::u8_view> {
        json coerce(utf::u8_view str) {
            return json(string(str.begin(), str.end()));
        }
    };


}


inline
fostlib::string::operator utf::u8_view () const  {
    return utf::u8_view(m_string);
}


#endif // FOST_UNICODE_HPP

