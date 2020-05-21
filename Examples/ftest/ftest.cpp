/**
    Copyright 2005-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <fost/dynlib>
#include <fost/main>
#include <fost/test>
#include <fost/unicode>


namespace {
    const fostlib::setting<bool> c_verbose("", "FTest", "Verbose", false, true);
}


FSL_MAIN(
        "ftest",
        "ftest - Fost test executor\n"
        "Copyright (c) 1995-2020 Red Anchor Trading Co. Ltd.")
(fostlib::ostream &out, fostlib::arguments &args) {
    args.commandSwitch("v", "Tests", "Display test names");
    if (args.size() < 2)
        throw fostlib::exceptions::null{
                "Must specify at least one DLL to load tests for"};
    else {
        std::list<boost::shared_ptr<fostlib::dynlib>> libraries;
        for (fostlib::arguments::size_type i(1); i < args.size(); ++i) {
            fostlib::fs::path extension =
                    fostlib::coerce<fostlib::fs::path>(args[i].value())
                            .extension();
            if (extension != ".lib" && extension != ".pdb")
                libraries.push_back(boost::shared_ptr<fostlib::dynlib>(
                        new fostlib::dynlib(args[i].value())));
        }
        if (fostlib::test::suite::execute(out))
            return 1;
        else
            return 0;
    }
}
