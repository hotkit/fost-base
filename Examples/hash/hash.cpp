/**
    Copyright 2013-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <fost/cli>
#include <fost/crypto>
#include <fost/file>
#include <fost/main>
#include <fost/progress-cli>
#include <fost/threading>


namespace {
    fostlib::string hash(fostlib::meter &, const fostlib::fs::path &file) {
        fostlib::digester hasher(fostlib::md5);
        hasher << file;
        return fostlib::coerce<fostlib::string>(
                fostlib::coerce<fostlib::hex_string>(hasher.digest()));
    }

    void
            process(std::ostream &out,
                    fostlib::meter &tracking,
                    fostlib::workerpool &pool,
                    const fostlib::fs::path path) {
        if (fostlib::fs::is_directory(path)) {
            for (fostlib::directory_iterator file(path);
                 file != fostlib::directory_iterator(); ++file) {
                process(out, tracking, pool, *file);
            }
        } else {
            fostlib::future<fostlib::string> md5_hash = pool.f<fostlib::string>(
                    [&tracking, path]() { return hash(tracking, path); });
            while (!md5_hash.available(boost::posix_time::milliseconds(50))) {
                fostlib::meter::reading current(tracking());
                std::cerr << "[" << fostlib::cli::bar(current, 38) << "] "
                          << path << "\r" << std::flush;
            }
            fostlib::meter::reading current(tracking());
            std::cerr << "[" << fostlib::cli::bar(current, 38) << "] " << path
                      << "\r\n";
            out << md5_hash() << "  " << path << std::endl;
        }
    }
}


FSL_MAIN("hash", "File hashing")
(fostlib::ostream &out, fostlib::arguments &args) {
    fostlib::meter tracking;
    fostlib::workerpool pool;
    for (std::size_t n(1); n < args.size(); ++n) {
        auto path(fostlib::coerce<fostlib::fs::path>(args[n].value()));
        process(out, tracking, pool, path);
    }
    return 0;
}
