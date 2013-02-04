/*
    Copyright 2013, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/cli>
#include <fost/crypto>
#include <fost/main>
#include <fost/progress-cli>
#include <fost/threading>
#include <fost/unicode>


using namespace fostlib;


namespace {
    string hash(meter &tracker, const boost::filesystem::wpath &file) {
        tracker.observe();
        digester hasher(md5);
        hasher << file;
        return coerce<string>(coerce<hex_string>(hasher.digest()));
    }
}


FSL_MAIN(
    L"hash",
    L"File hashing"
)( ostream &out, arguments &args ) {
    meter tracking;
    workerpool pool;
    for ( std::size_t n(1); n < args.size(); ++n ) {
        boost::filesystem::wpath path(
            coerce<boost::filesystem::wpath>(args[n].value()));
        if ( !boost::filesystem::is_directory(path) ) {
            future<string> md5_hash = pool.f<string>(
                boost::lambda::bind(hash, boost::ref(tracking), path));
            while ( !md5_hash.available() ) {
                boost::this_thread::sleep(boost::posix_time::milliseconds(200));
                meter::reading current(tracking());
                std::cerr << "[" << cli::bar(current, 38) << "] " <<
                    current.done() << " " << path << "\r" << std::flush;
            }
            std::cerr << std::endl;
            out << md5_hash() << "  " << path << std::endl;
        }
    }
    return 0;
}

