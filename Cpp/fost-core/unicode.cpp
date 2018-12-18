/**
    Copyright 2001-2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-core.hpp"
#include <fost/insert.hpp>
#include <fost/unicode.hpp>

#include <fost/exception/not_implemented.hpp>
#include <fost/exception/out_of_range.hpp>


using namespace fostlib;


/**
    ## Misc encoding & decoding functions
*/


std::size_t fostlib::utf::length(nliteral seq) {
    std::size_t count = 0;
    for (; *seq != 0; ++count) {
        std::size_t chars =
                f5::cord::u8codepoint_length<exceptions::unicode_encoding>(
                        *seq);
        for (std::size_t chk = 1; chk < chars; chk++) {
            unsigned char current(
                    *reinterpret_cast<const unsigned char *>(seq + chk));
            if (current < 0x80 || current > 0xBF)
                throw fostlib::exceptions::unicode_encoding(
                        L"UTF-8 continuation character is not correct ("
                        + coerce<string>(chk) + L" of " + coerce<string>(chars)
                        + L") is " + coerce<string>(int(current)));
        }
        seq += chars;
    }
    return count;
}
std::size_t fostlib::utf::length(wliteral seq) {
    std::size_t count = 0;
    for (; *seq != 0; ++count) {
        if (*seq >= 0xD800 && *seq <= 0xDBFF) {
            ++seq;
            if (*seq < 0xDC00 || *seq > 0xDFFF)
                throw fostlib::exceptions::unicode_encoding(
                        L"Trailing character in a UTF-16 surrogate pair is "
                        L"missing");
            ++seq;
        } else if (*seq >= 0xDC00 && *seq <= 0xDFFF)
            throw fostlib::exceptions::unicode_encoding(
                    L"Trailing character in a UTF-16 surrogate pair has been "
                    L"found first ("
                    + coerce<string>(int(*seq)) + L")");
        else
            ++seq;
    }
    return count;
}
std::size_t fostlib::utf::length(wliteral seq, wliteral end) {
    std::size_t count = 0;
    for (; *seq != 0 && seq != end; ++count) {
        if (*seq >= 0xD800 && *seq <= 0xDBFF) {
            ++seq;
            if (seq == end)
                return count - 1;
            else if (*seq < 0xDC00 || *seq > 0xDFFF)
                throw fostlib::exceptions::unicode_encoding(
                        L"Trailing character in a UTF-16 surrogate pair is "
                        L"missing");
            ++seq;
        } else if (*seq >= 0xDC00 && *seq <= 0xDFFF)
            throw fostlib::exceptions::unicode_encoding(
                    L"Trailing character in a UTF-16 surrogate pair has been "
                    L"found first ("
                    + coerce<string>(int(*seq)) + L")");
        else
            ++seq;
    }
    return count;
}


namespace {
    template<typename C>
    std::size_t character_length(utf32 ch);

    template<>
    std::size_t character_length<char>(utf32 ch) {
        utf::assertValid(ch);
        if (ch < 0x00080)
            return 1;
        else if (ch < 0x00800)
            return 2;
        else if (ch < 0x10000)
            return 3;
        else
            return 4;
    }

    template<>
    std::size_t character_length<utf16>(utf32 ch) {
        utf::assertValid(ch);
        if (ch < 0x10000)
            return 1;
        else
            return 2;
    }
}
std::size_t fostlib::utf::utf16length(utf32 ch) {
    return character_length<utf16>(ch);
}
std::size_t fostlib::utf::native_length(utf32 ch) {
    return character_length<native_char>(ch);
}


std::size_t fostlib::utf::native_length(nliteral) {
    throw fostlib::exceptions::not_implemented(
            L"std::size_t fostlib::utf::native_length( const utf8 * )");
}
std::size_t fostlib::utf::native_length(wliteral) {
    throw fostlib::exceptions::not_implemented(
            L"std::size_t fostlib::utf::native_length( wliteral )");
}


utf32 fostlib::utf::decode(nliteral seq, nliteral end) {
    f5::cord::const_u8buffer buffer(
            reinterpret_cast<const f5::cord::utf8 *>(seq),
            reinterpret_cast<const f5::cord::utf8 *>(end));
    const auto result = f5::cord::decode_one<
            exceptions::unicode_encoding, exceptions::unicode_encoding>(buffer);
    const utf32 ch = result.first;
    const std::size_t bytes = buffer.size() - result.second.size();
    if (utf8length(ch) != bytes)
        throw fostlib::exceptions::unicode_encoding(
                L"UTF-8 sequence of " + coerce<string>(bytes)
                + L" chars generated a UTF32 character ("
                + coerce<string>(int(ch)) + L") with a different length ("
                + coerce<string>(utf8length(ch)) + L")");
    return ch;
}
utf32 fostlib::utf::decode(wliteral begin, wliteral end) {
    if (begin == 0)
        return utf32(0);
    else if (begin + 1 == end)
        return utf::decode(*begin);
    else
        return utf::decode(*begin, *(begin + 1));
}
utf32 fostlib::utf::decode(wchar_t first) { return utf::decode(first, 0); }
utf32 fostlib::utf::decode(wchar_t first, wchar_t second) {
    try {
        utf32 ch = first;
        if (ch >= 0xffff)
            throw fostlib::exceptions::unicode_encoding(
                    L"This character is outside the allowed range for a single "
                    L"UTF-16 wchar_t");
        else if (ch >= 0xD800 && ch <= 0xDBFF) {
            if (second == 0)
                throw fostlib::exceptions::unicode_encoding(
                        L"Trailing surrogate missing from UTF-16 sequence (it "
                        L"is ZERO)");
            if (second < 0xDC00 || second > 0xDFFF)
                throw fostlib::exceptions::unicode_encoding(
                        L"Trailing character in a UTF-16 surrogate pair is "
                        L"missing (outside correct range)");
            return assertValid(
                    (ch << 10) + second + 0x10000 - (0xD800 << 10) - 0xDC00);
        }
        return assertValid(ch);
    } catch (fostlib::exceptions::exception &e) {
        fostlib::insert(e.data(), "utf-16", "decoding", first);
        fostlib::insert(e.data(), "utf-16", "following", second);
        throw;
    }
}


std::size_t fostlib::utf::encode(utf32 ch, utf16 *begin, const utf16 *end) {
    try {
        auto encoded =
                f5::cord::u16encode<fostlib::exceptions::unicode_encoding>(ch);
        /// The `<= end` here looks wrong, but is right.
        if (begin + encoded.first <= end) {
            begin[0] = encoded.second[0];
            if (encoded.first == 2) { begin[1] = encoded.second[1]; }
            return encoded.first;
        } else {
            throw exceptions::out_of_range<std::size_t>(
                    "Buffer is not long enough to hold the UTF-16 sequence for "
                    "this character",
                    encoded.first, std::numeric_limits<std::size_t>::max(),
                    end - begin);
        }
    } catch (exceptions::exception &e) {
        insert(e.data(), "code-point", ch);
        throw;
    }
}
