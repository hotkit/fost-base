/*
    Copyright 2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/log>


using namespace fostlib;


FSL_TEST_SUITE( log );


FSL_TEST_FUNCTION( message ) {
    const logging::message m1("testsuite-log", 12, "test-level", json(true));
    FSL_CHECK_EQ(m1.module().value(), "testsuite-log");
    FSL_CHECK_EQ(m1.level(), 12u);
    FSL_CHECK_EQ(m1.name(), "test-level");
    FSL_CHECK_EQ(m1.body(), json(true));
    const json m1js = coerce<json>(m1);
    FSL_CHECK(m1js.has_key("when"));
    FSL_CHECK_EQ(m1js["module"], json("testsuite-log"));
    FSL_CHECK_EQ(m1js["level"]["value"], json(12u));
    FSL_CHECK_EQ(m1js["level"]["name"], json("test-level"));
    FSL_CHECK_EQ(m1js["body"], json(true));


    const logging::message m2(1234, "test-level2", json(false));
    FSL_CHECK(m2.module().isnull());
    FSL_CHECK_EQ(m2.level(), 1234u);
    FSL_CHECK_EQ(m2.name(), "test-level2");
    FSL_CHECK_EQ(m2.body(), json(false));
    const json m2js = coerce<json>(m2);
    FSL_CHECK(m2js.has_key("when"));
    FSL_CHECK_EQ(m2js["module"], json());
    FSL_CHECK_EQ(m2js["level"]["value"], json(1234u));
    FSL_CHECK_EQ(m2js["level"]["name"], json("test-level2"));
    FSL_CHECK_EQ(m2js["body"], json(false));
}


#include <iostream>
namespace {
    struct capture_copy {
        typedef fostlib::json result_type;
        result_type messages;
        bool log(const fostlib::logging::message &m) {
            using namespace fostlib;
            std::cout << coerce<json>(m) << std::endl;
            push_back(messages, coerce<json>(m));
            return true;
        }
        result_type operator () () const {
            return messages;
        }
    };
}
FSL_TEST_FUNCTION( log ) {
    using namespace fostlib::logging;
    fostlib::logging::scoped_sink< capture_copy > cc;
    log(debug, "Debug level");
    log(info, "Info level");
    log(warning, "Warning level");
    log(error, "Error level");
    log(critical, "Critical level");
    fostlib::json data = cc();
    FSL_CHECK_EQ(data, fostlib::json());
    FSL_CHECK_EQ(data.size(), 5u);
}


FSL_TEST_FUNCTION( direct ) {
    using namespace fostlib::logging;
    debug("Debug level");
    info("Info level");
    warning("Warning level");
    error("Error level");
    critical("Critical level");
}
