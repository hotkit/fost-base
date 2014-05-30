/*
    Copyright 2010-2014, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-cli.hpp"
#include <fost/log>

#include <iostream>


namespace {
    class ostream_logger {
        std::size_t log_level;
        public:
            ostream_logger(const fostlib::json &conf) {
                log_level = fostlib::coerce< fostlib::nullable<int> >(
                        conf["log-level"]).value(
                    fostlib::log::error_level_tag::level());
            }
            bool operator () ( const fostlib::log::message &m ) {
#ifdef FOST_OS_WINDOWS
    #define COUT std::wcout
#else
    #define COUT std::cout
#endif
                if ( m.level() >= log_level ) {
                    COUT << m.when() << " " << m.name();
                    if ( !m.module().isnull() ) {
                        COUT<< " " << m.module().value();
                    }
                    COUT<< '\n' << m.body() << std::endl;
                }
                return true;
            }
    };

    const fostlib::log::global_sink< ostream_logger > std_out("stdout");
}

