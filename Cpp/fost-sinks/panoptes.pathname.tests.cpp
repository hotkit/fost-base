/**
    Copyright 2016, Felspar Co Ltd. <http://support.felspar.com/>

    Copyright 2012-2015, Proteus Technologies Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include <fost/sinks>
#include <fost/test>


namespace {
    const fostlib::module c_fost_sinks_test(fostlib::c_fost_sinks, "test");
    const fostlib::module c_fost_sinks_test_f(c_fost_sinks_test,  __FILE__);
}


FSL_TEST_SUITE(archive_pathname);


FSL_TEST_FUNCTION(filename) {
    fostlib::log::detail::archive_pathname filename, module(c_fost_sinks_test_f);
    fostlib::timestamp w(2011, 8, 15, 14, 54);

    FSL_CHECK_EQ(filename(w),
        "var/log/fost"
        "/2011-08/15/--unknown"
        "/2011-08-15T145400Z.jsonl");

    FSL_CHECK_EQ(module(w),
        "var/log/fost"
        "/2011-08/15/fost/sinks/test/" __FILE__
        "/2011-08-15T145400Z.jsonl");
}


FSL_TEST_FUNCTION( rotate ) {
    fostlib::log::detail::archive_pathname logger;
    FSL_CHECK_EQ(logger.rotate(0), false);
    FSL_CHECK_EQ(logger.rotate(1024), false);
    FSL_CHECK_EQ(logger.rotate(1024 * 10), false);
    FSL_CHECK_EQ(logger.rotate(1024 * 100), false);
    FSL_CHECK_EQ(logger.rotate(1024 * 127), false);
    FSL_CHECK_EQ(logger.rotate(1024 * 127 + 256), false);
    FSL_CHECK_EQ(logger.rotate(1024 * 127 + 512), false);
    FSL_CHECK_EQ(logger.rotate(1024 * 127 + 1023), false);
    FSL_CHECK_EQ(logger.rotate(1024 * 128), true);
    FSL_CHECK_EQ(logger.rotate(1024 * 200), true);
    FSL_CHECK_EQ(logger.rotate(uintmax_t(1) << 40), true);
}

