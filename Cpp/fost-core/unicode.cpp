/**
    Copyright 2001-2020 Red Anchor Trading Co. Ltd.

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
                        "UTF-8 continuation character is not correct ("
                        + coerce<string>(chk) + " of " + coerce<string>(chars)
                        + ") is " + coerce<string>(int(current)));
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
                        "Trailing character in a UTF-16 surrogate pair is "
                        "missing");
            ++seq;
        } else if (*seq >= 0xDC00 && *seq <= 0xDFFF)
            throw fostlib::exceptions::unicode_encoding(
                    "Trailing character in a UTF-16 surrogate pair has been "
                    "found first ("
                    + coerce<string>(int(*seq)) + ")");
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
                        "Trailing character in a UTF-16 surrogate pair is "
                        "missing");
            ++seq;
        } else if (*seq >= 0xDC00 && *seq <= 0xDFFF)
            throw fostlib::exceptions::unicode_encoding(
                    "Trailing character in a UTF-16 surrogate pair has been "
                    "found first ("
                    + coerce<string>(int(*seq)) + ")");
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
                "UTF-8 sequence of " + coerce<string>(bytes)
                + " chars generated a UTF32 character ("
                + coerce<string>(int(ch)) + ") with a different length ("
                + coerce<string>(utf8length(ch)) + ")");
    return ch;
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
