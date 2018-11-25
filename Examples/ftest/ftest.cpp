/*
    Copyright 2005-2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/dynlib>
#include <fost/main>
#include <fost/test>
#include <fost/unicode>


namespace {
    const fostlib::setting<bool>
            c_verbose(L"", L"FTest", L"Verbose", false, true);
}


FSL_MAIN(
        L"ftest",
        L"ftest - Fost test executor\nCopyright (c) 1995-2010, Felspar Co. "
        L"Ltd.")
(fostlib::ostream &out, fostlib::arguments &args) {
    args.commandSwitch(L"v", L"Tests", L"Display test names");
    if (args.size() < 2)
        throw fostlib::exceptions::null(
                L"Must specify at least one DLL to load tests for");
    else {
        std::list<boost::shared_ptr<fostlib::dynlib>> libraries;
        for (fostlib::arguments::size_type i(1); i < args.size(); ++i) {
            boost::filesystem::wpath extension =
                    fostlib::coerce<boost::filesystem::wpath>(args[i].value())
                            .extension();
            if (extension != L".lib" && extension != L".pdb")
                libraries.push_back(boost::shared_ptr<fostlib::dynlib>(
                        new fostlib::dynlib(args[i].value())));
        }
        if (fostlib::test::suite::execute(out))
            return 1;
        else
            return 0;
    }
}
