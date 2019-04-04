/**
    Copyright 2016-2019, Felspar Co Ltd. <http://support.felspar.com/>

    Copyright 2012-2015, Proteus Technologies Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include <fost/filesystem.hpp>
#include <fost/sinks.panoptes.hpp>


namespace {
    const fostlib::log::global_sink<fostlib::log::panoptes>
            c_logger("panoptes");
}


fostlib::log::panoptes::panoptes(const fostlib::json &) {}


bool fostlib::log::panoptes::operator()(const fostlib::log::message &m) {
    fostlib::string entry(
            fostlib::json::unparse(fostlib::coerce<fostlib::json>(m), false));
    const auto *modp = &m.module();

    fostlib::fs::path filename;
    auto archive = logfile_pathnames.find(modp);
    if (archive == logfile_pathnames.end()) {
        filename =
                (logfile_pathnames[modp] =
                         detail::archive_pathname(m.module()))(m.when());
    } else {
        filename = (archive->second)(m.when());
    }

    fostlib::fs::ofstream file(
            filename, std::ios_base::out | std::ios_base::app);
    file << entry.c_str() << std::endl;

    return true;
}
