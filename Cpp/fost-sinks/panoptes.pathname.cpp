/**
    Copyright 2016-2019, Felspar Co Ltd. <http://support.felspar.com/>

    Copyright 2012-2015, Proteus Technologies Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include <fost/sinks.panoptes.hpp>


namespace {
    const fostlib::module c_legacy("-unnamed-");
}


fostlib::log::detail::archive_pathname::archive_pathname()
: modulep(&c_legacy), max_size_kb(128) {}


fostlib::log::detail::archive_pathname::archive_pathname(const class module &m)
: modulep(&m), max_size_kb(128) {}


fostlib::log::detail::archive_pathname::fileloc_type
        fostlib::log::detail::archive_pathname::pathname(
                const fostlib::timestamp &when) const {
    fostlib::string ts = fostlib::replace_all(coerce<string>(when), ":", null);
    fostlib::fs::wpath directory =
            coerce<fostlib::fs::wpath>(c_log_sink_file_root.value())
            / coerce<fostlib::fs::wpath>(ts.substr(0, 7))
            / coerce<fostlib::fs::wpath>(ts.substr(8, 2));
    fostlib::fs::wpath data_path(
            directory
            / coerce<fostlib::fs::wpath>(
                      modulep->as_string() + "/" + ts + ".jsonl"));
    if (!fostlib::fs::exists(data_path.parent_path())) {
        fostlib::fs::create_directories(data_path.parent_path());
    }
    date day(coerce<boost::posix_time::ptime>(when).date());
    fileloc_type fl = {day, data_path};
    return fl;
}


fostlib::fs::wpath fostlib::log::detail::archive_pathname::
        operator()(const fostlib::timestamp &when) {
    const boost::posix_time::ptime time(coerce<boost::posix_time::ptime>(when));
    const date day(time.date());
    if (not fileloc) {
        fileloc = pathname(when);
    } else if (fostlib::fs::exists(fileloc.value().pathname)) {
        uintmax_t size = fostlib::fs::file_size(fileloc.value().pathname);
        if (rotate(size) || day != fileloc.value().date) {
            fileloc = pathname(when);
        }
    }
    return fileloc.value().pathname;
}


bool fostlib::log::detail::archive_pathname::rotate(uintmax_t size) {
    const uintmax_t kb = size >> 10u;
    return kb >= max_size_kb;
}
