/*
    Copyright 1999-2018,Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/

#include "fost-core.hpp"
#include <fost/detail/utility.hpp>
#include <fost/detail/coerce.hpp>
#include <fost/trim.hpp>
#include <ctype.h>

// Function for checking unicode spaces
bool is_whitespace(unsigned int text){

    const char32_t WHITESPACES [] = {0x0020, 0x00A0, 0x1680, 0x180E,
                                     0x2000, 0x2001, 0x2002, 0x2003,
                                     0x2004, 0x2005, 0x2006, 0x2007,
                                     0x2008, 0x2009, 0x200A, 0x200B,
                                     0x202F, 0x205F, 0x3000, 0xFEFF};

    for ( int i = 0; i < sizeof(WHITESPACES)/sizeof(WHITESPACES[0]); i++ ) {
        if ( WHITESPACES[i] == text ) { return true; }
    }

    return false;
}

// Function for trimming leading and trailing whitespaces
f5::u8view fostlib::trim(f5::u8view text){

    int first_not_of = 0, last_not_of = 0, counter = 1;
    bool stop_flag = false;

    // If the string is empty return itself
    if (text.empty()) {
        return text;
    }

    // Iterate through string once and get the first position that is not whitespace and last position before whitespace for slicing
    for (auto pos = text.begin(), end = text.end(); pos != end; ++pos) {

        // Check if current character is a whitespace (defined by both system locale and our function)
        if (isspace((wchar_t)*pos, std::locale("en_US.UTF8")) || is_whitespace(*pos)) {
            // If we have yet to find a nonwhitespace, increment the first position that is not whitespace
            if (!stop_flag) {
                first_not_of = counter;
            }
        }else { // Otherwise, stop incrementing first position and update the last position
            last_not_of = counter;
            stop_flag = true;
        }

        counter++;
    }

    return text.substr_pos(first_not_of, last_not_of);
}