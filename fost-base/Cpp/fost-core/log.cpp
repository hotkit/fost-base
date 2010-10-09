/*
    Copyright 2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/log.hpp>

#include <deque>


using namespace fostlib;


namespace {
    struct message {
        message(std::size_t l, nliteral n, const json &j)
        : level(l), name(n), body(j) {
        }

        accessors< std::size_t > level;
        accessors< nliteral > name;
        accessors< json > body;
    };

    class log_proxy {
        class log_queue {
            std::deque< message > queue;
        };
    };
}


void fostlib::logging::log(std::size_t l, nliteral n, const json &j) {
    const message m(l, n, j);
}
