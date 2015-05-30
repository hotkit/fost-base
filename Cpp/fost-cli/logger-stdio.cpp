/*
    Copyright 2010-2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-cli.hpp"
#include <fost/log>

#include <iostream>


namespace {
    class ostream_logger {
        const std::size_t log_level;
        const bool colour;
        public:
            ostream_logger(const fostlib::json &conf)
            : log_level(fostlib::coerce<fostlib::nullable<int>>(
                        conf["log-level"]).value(
                            fostlib::log::error_level_tag::level())),
                    colour(fostlib::coerce<fostlib::nullable<bool>>(
                        conf["color"]).value(false)) {
            }
            bool operator () ( const fostlib::log::message &m ) {
#ifdef FOST_OS_WINDOWS
    #define COUT std::wcout
#else
    #define COUT std::cout
#endif
                if ( colour ) {
                    if ( m.level() <= fostlib::log::debug_level_tag::level() ) {
                        COUT << "\33[0;37m";
                    } else if ( m.level() <= fostlib::log::info_level_tag::level() ) {
                        COUT << "\33[0;32m";
                    } else if ( m.level() <= fostlib::log::warning_level_tag::level() ) {
                        COUT << "\33[0;33m";
                    } else if ( m.level() <= fostlib::log::error_level_tag::level() ) {
                        COUT << "\33[0;31m";
                    } else {
                        COUT << "\33[1;31m";
                    }
                }
                if ( m.level() >= log_level ) {
                    COUT << m.when() << " " << m.name();
                    if ( !m.module().isnull() ) {
                        COUT<< " " << m.module().value();
                    }
                    COUT<< '\n' << m.body() << std::endl;
                }
                if ( colour ) {
                    COUT << "\33[0;39m";
                }
                return true;
            }
    };

    const fostlib::log::global_sink< ostream_logger > std_out("stdout");
}

