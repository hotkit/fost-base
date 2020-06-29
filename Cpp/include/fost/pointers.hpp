/**
    Copyright 2001-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <fost/core>
#include <fost/exception/null.hpp>
#include <fost/pointers.shared.hpp>


namespace fostlib {


    typedef std::pair<void *, void *> memory_block;
    typedef std::pair<const void *, const void *> const_memory_block;


    template<>
    struct FOST_CORE_DECLSPEC coercer<utf8_string, const_memory_block> {
        utf8_string coerce(const const_memory_block &);
    };


    /// The number of bytes in the memory block
    inline std::size_t bytes(memory_block b) {
        return reinterpret_cast<const char *>(b.second)
                - reinterpret_cast<const char *>(b.first);
    }
    /// The number of bytes in the memory block
    inline std::size_t bytes(const_memory_block b) {
        return reinterpret_cast<const char *>(b.second)
                - reinterpret_cast<const char *>(b.first);
    }


}
