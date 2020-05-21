/**
    Copyright 2009-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <fost/cli>
#include <fost/filesystem.hpp>
#include <fost/main>
#include <fost/unicode>


FSL_MAIN(
        "fost-core-test-file-io",
        "fost-core-test-file-io\n"
        "Test file I/O and its Unicode handling\n"
        "Copyright (C) 2009-2020 Red Anchor Trading Co. Ltd.")
(fostlib::ostream &out, fostlib::arguments &args) {
    auto filename(fostlib::coerce<fostlib::fs::path>(args[1].value()));
    // Check that we can do some basic reads
    { // Build a basic text stream that we want to check against
        fostlib::ofstream outfile(filename);
        outfile.write("abcdef\n", 7);
        unsigned char tm[] = {0xe2, 0x84, 0xa2, 0x00};
        outfile.write(reinterpret_cast<char *>(tm), 3);
        outfile.write("\n", 1);
    }
    f5::u8string loaded = fostlib::utf::load_file(filename);
    if (loaded != f5::u8string{u"abcdef\n\x2122\n"}) {
        out << "File loaded did not match file saved\n"
               "97 98 99 100 101 102 10 8482 10"
            << std::endl;
        for (auto const c : loaded) { out << int(c) << " "; }
        out << std::endl;
        return 1;
    }
    fostlib::fs::remove(filename);
    return 0;
}
