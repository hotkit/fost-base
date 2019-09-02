/**
    Copyright 2010-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-core-test.hpp"


#include <fost/yaml.hpp>
#include <fost/exception/parse_error.hpp>


FSL_TEST_SUITE(yaml);


FSL_TEST_FUNCTION(adding) {
    fostlib::yaml::record r, r2;

    FSL_CHECK_NOTHROW(r.add("f1").add("f2", fostlib::null));
    FSL_CHECK_NOTHROW(r.add("f3", fostlib::string("v")));
    FSL_CHECK_NOTHROW(r.add("f4", 10));
    FSL_CHECK_NOTHROW(r.add("f5", fostlib::nullable<int>())
                              .add("f6", fostlib::nullable<int>(123)));
    FSL_CHECK_NOTHROW(
            r.add("f7", fostlib::nullable<fostlib::string>())
                    .add("f8", fostlib::nullable<fostlib::string>("v6")));
    FSL_CHECK_EXCEPTION(r.add("f9", r), fostlib::exceptions::not_implemented &);

    FSL_CHECK_NOTHROW(r2.add("F1").add("F2", fostlib::null));
    FSL_CHECK_NOTHROW(r.add("f10", r2));
}
