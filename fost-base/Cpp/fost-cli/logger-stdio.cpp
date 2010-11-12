/*
    Copyright 2010, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-cli.hpp"
#include <fost/log>


namespace {
    class ostream_logger {
        public:
            ostream_logger(const fostlib::json &) {
            }
            bool operator () ( const fostlib::logging::message &m ) {
                std::cout << m.when() << " " << m.name();
                if ( !m.module().isnull() )
                    std::cout << " " << m.module().value();
                std::cout << '\n' << m.body() << std::endl;
                return true;
            }
    };

    const fostlib::logging::global_sink< ostream_logger > std_out("stdout");
}

