/**
    Copyright 2013-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-cli.hpp"
#include <fost/progress-cli>


using namespace fostlib;


string fostlib::cli::bar(const meter::reading &r, const std::size_t width) {
    if (r.is_complete()) {
        return string(width, '.');
    } else if (not r.work().value_or(0)) {
        return string(width, '-');
    } else {
        const std::size_t dots(r.done() * width / *r.work());
        return string(dots, '.') + string(width - dots, ' ');
    }
}


string fostlib::cli::detail::prefix(const meter::reading &) { return "["; }


string fostlib::cli::detail::suffix(const meter::reading &) { return "]"; }
