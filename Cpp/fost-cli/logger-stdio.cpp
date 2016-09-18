/*
    Copyright 2010-2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-cli.hpp"
#include <fost/log>

#include <iostream>


#ifdef FOST_OS_WINDOWS
    #define COUT std::wcout
#else
    #define COUT std::cout
#endif


namespace {
    struct disp {
        typedef void result_type;
        template<typename T>
        void operator () (const T &t) const {
            COUT << ' ' << t << '\n' << std::endl;
        }
        void operator () (const fostlib::string &s) const {
            COUT << s << std::endl;
        }
        void operator () (const fostlib::json::object_t &o) const {
            if ( o.find("") != o.end() ) {
                fostlib::json copy = o;
                fostlib::string m = fostlib::coerce<fostlib::string>(copy[""]);
                fostlib::jcursor("").del_key(copy);
                COUT << ' ' << m << '\n' << copy << std::endl;
            } else {
                COUT << '\n' << o << std::endl;
            }
        }
        void operator () (const fostlib::json::array_t &a) const {
            if ( a.size() > 0 && a[0]->isatom() ) {
                COUT << ' ' << fostlib::coerce<fostlib::string>(*a[0]) << '\n';
                for ( std::size_t i(1); i != a.size(); ++i ) {
                    COUT << *a[i] << '\n';
                }
                COUT << std::endl;
            } else {
                COUT << '\n' << a << std::endl;
            }
        }
    };

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
                if ( colour ) {
                    if ( m.level() <= fostlib::log::debug_level_tag::level()  + 0x100 ) {
                        COUT << "\33[0;37m";
                    } else if ( m.level() <= fostlib::log::info_level_tag::level() + 0x300 ) {
                        COUT << "\33[0;32m";
                    } else if ( m.level() <= fostlib::log::warning_level_tag::level() + 0x2000 ) {
                        COUT << "\33[1;33m";
                    } else if ( m.level() <= fostlib::log::error_level_tag::level() + 0x20000 ) {
                        COUT << "\33[0;31m";
                    } else {
                        COUT << "\33[1;31m";
                    }
                }
                if ( m.level() >= log_level ) {
                    COUT << m.when() << " " << m.name() << " " << m.module();
                    if ( colour ) {
                        disp d;
                        m.body().apply_visitor(d);
                    } else {
                        COUT<< '\n' << m.body() << std::endl;
                    }
                }
                if ( colour ) {
                    COUT << "\33[0;39m";
                }
                return true;
            }
    };

    const fostlib::log::global_sink< ostream_logger > std_out("stdout");
}

