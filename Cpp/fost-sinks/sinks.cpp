/*
    Copyright 2016, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/sinks.hpp>


const fostlib::module fostlib::c_fost_sinks(c_fost, "sinks");


const fostlib::setting<fostlib::string> fostlib::c_log_sink_file_root(
        "sinks.cpp", "Log sinks", "Log files directory", "var/log/fost", true);
