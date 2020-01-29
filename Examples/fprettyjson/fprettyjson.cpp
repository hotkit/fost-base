/**
    Copyright 2008-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <fost/cli>
#include <fost/main>
#include <fost/unicode>

#include <fost/parse/json.hpp>
#include <fost/exception/parse_error.hpp>


using namespace fostlib;


namespace {
    setting<bool> c_overwrite(
            "fprettyjson/fprettyjson.cpp",
            "fprettyjson",
            "Overwrite original",
            false,
            true);
}


FSL_MAIN(
        "fprettyjson",
        "JSON pretty printer\nCopyright (c) 2008-2020, Red Anchor Trading Co. "
        "Ltd.")
(fostlib::ostream &out, fostlib::arguments &args) {
    /**
        Process the extra command line switches we want
    */
    args.commandSwitch("o", "fprettyjson", "Overwrite original");
    args.commandSwitch("s", "JSON", "Unparse tab width");
    /**
        Check that we have an input file name
    */
    if (not args[1]) {
        out << "No JSON file to load was specified" << std::endl;
        return 1;
    }
    /**
        Load the JSON object
    */
    auto const content = utf::load_file(coerce<fs::path>(args[1].value()));
    auto const blob{fostlib::json::parse(content)};
    /**
        If we don't have a 2nd filename and we're not overwriting then output to
       the screen
    */
    if (not args[2] && not c_overwrite.value()) {
        out << json::unparse(blob, true) << std::endl;
    } else {
        /**
            Otherwise output to the 2nd argument or back to the 1st
        */
        string ofile(args[2].value_or(args[1].value()));
        utf::save_file(coerce<std::wstring>(ofile), json::unparse(blob, true));
        out << "JSON saved to " << ofile << std::endl;
    }
    return 0;
}
