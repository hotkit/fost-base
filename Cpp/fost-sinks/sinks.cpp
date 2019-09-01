/**
    Copyright 2016-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <fost/sinks.hpp>


const fostlib::module fostlib::c_fost_sinks(c_fost, "sinks");


const fostlib::setting<fostlib::string> fostlib::c_log_sink_file_root(
        "sinks.cpp", "Log sinks", "Log files directory", "var/log/fost", true);
