/*
    Copyright 2010-2012, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/log>
#include <fost/insert>


using namespace fostlib;


FSL_TEST_SUITE( log );


FSL_TEST_FUNCTION( message ) {
    fostlib::nliteral test_level = "test-level";
    const logging::message m1("testsuite-log", 12, test_level, json(true));
    FSL_CHECK_EQ(m1.module().value(), "testsuite-log");
    FSL_CHECK_EQ(m1.level(), 12u);
    FSL_CHECK_EQ(m1.name(), test_level);
    FSL_CHECK_EQ(m1.body(), json(true));
    const json m1js = coerce<json>(m1);
    FSL_CHECK(m1js.has_key("when"));
    FSL_CHECK_EQ(m1js["module"], json("testsuite-log"));
    FSL_CHECK_EQ(m1js["level"]["value"], json(12u));
    FSL_CHECK_EQ(m1js["level"]["name"], json("test-level"));
    FSL_CHECK_EQ(m1js["body"], json(true));

    fostlib::nliteral test_level2 = "test-level2";
    const logging::message m2(1234, test_level2, json(false));
    FSL_CHECK(m2.module().isnull());
    FSL_CHECK_EQ(m2.level(), 1234u);
    FSL_CHECK_EQ(m2.name(), test_level2);
    FSL_CHECK_EQ(m2.body(), json(false));
    const json m2js = coerce<json>(m2);
    FSL_CHECK(m2js.has_key("when"));
    FSL_CHECK(!m2js.has_key("module"));
    FSL_CHECK_EQ(m2js["level"]["value"], json(1234u));
    FSL_CHECK_EQ(m2js["level"]["name"], json("test-level2"));
    FSL_CHECK_EQ(m2js["body"], json(false));
}


namespace {
    class capture_copy {
        bool pass_on;
        fostlib::json messages;
        public:
            capture_copy(bool pass_on = true)
            : pass_on(pass_on) {
            }
            typedef fostlib::json result_type;

            bool operator () (const fostlib::logging::message &m) {
                using namespace fostlib;
                push_back(messages, coerce<json>(m));
                return pass_on;
            }
            result_type operator () () const {
                return messages;
            }
    };
}
FSL_TEST_FUNCTION( capture_copy ) {
    fostlib::logging::scoped_sink< capture_copy > cc1, cc2(false);
    fostlib::logging::info("Log message");
    fostlib::json d1 = cc1(), d2 = cc2();
    FSL_CHECK_EQ(d1.size(), 0u);
    FSL_CHECK_EQ(d2.size(), 1u);
    FSL_CHECK_EQ(d2[0]["body"], fostlib::json("Log message"));
}
FSL_TEST_FUNCTION( log ) {
    using namespace fostlib::logging;
    fostlib::logging::scoped_sink< capture_copy > cc;
    log(debug, "Example debug message -- please ignore");
    log(info, "Example info message -- please ignore");
    log(warning, "Example warning message -- please ignore");
    log(error, "Example error message -- please ignore");
    log(critical, "Example critical message -- please ignore");
    fostlib::json data = cc();
    FSL_CHECK_EQ(data.size(), 5u);
    FSL_CHECK_EQ(data[0]["body"],
        fostlib::json("Example debug message -- please ignore"));
    FSL_CHECK_EQ(data[0]["level"]["name"], fostlib::json("debug"));
    FSL_CHECK_EQ(data[1]["body"],
        fostlib::json("Example info message -- please ignore"));
    FSL_CHECK_EQ(data[1]["level"]["name"], fostlib::json("info"));
    FSL_CHECK_EQ(data[2]["body"],
        fostlib::json("Example warning message -- please ignore"));
    FSL_CHECK_EQ(data[2]["level"]["name"], fostlib::json("warning"));
    FSL_CHECK_EQ(data[3]["body"],
        fostlib::json("Example error message -- please ignore"));
    FSL_CHECK_EQ(data[3]["level"]["name"], fostlib::json("error"));
    FSL_CHECK_EQ(data[4]["body"],
        fostlib::json("Example critical message -- please ignore"));
    FSL_CHECK_EQ(data[4]["level"]["name"], fostlib::json("critical"));
}


FSL_TEST_FUNCTION( direct ) {
    using namespace fostlib::logging;
    scoped_sink< capture_copy > cc;
    debug("Example debug message -- please ignore");
    info("Example info message -- please ignore");
    warning("Example warning message -- please ignore");
    error("Example error message -- please ignore");
    critical("Example critical message -- please ignore");
    fostlib::json data = cc();
    FSL_CHECK_EQ(data[0]["body"],
        fostlib::json("Example debug message -- please ignore"));
    FSL_CHECK_EQ(data[0]["level"]["name"], fostlib::json("debug"));
    FSL_CHECK_EQ(data[1]["body"],
        fostlib::json("Example info message -- please ignore"));
    FSL_CHECK_EQ(data[1]["level"]["name"], fostlib::json("info"));
    FSL_CHECK_EQ(data[2]["body"],
        fostlib::json("Example warning message -- please ignore"));
    FSL_CHECK_EQ(data[2]["level"]["name"], fostlib::json("warning"));
    FSL_CHECK_EQ(data[3]["body"],
        fostlib::json("Example error message -- please ignore"));
    FSL_CHECK_EQ(data[3]["level"]["name"], fostlib::json("error"));
    FSL_CHECK_EQ(data[4]["body"],
        fostlib::json("Example critical message -- please ignore"));
    FSL_CHECK_EQ(data[4]["level"]["name"], fostlib::json("critical"));
}


FSL_TEST_FUNCTION( direct_with_nullables ) {
    using namespace fostlib::logging;
    scoped_sink< capture_copy > cc;
    nullable<string> empty;
    debug(empty);
    fostlib::json data = cc();
    FSL_CHECK_EQ(data[0]["body"], fostlib::json());
}


FSL_TEST_FUNCTION( direct_with_multiple_arguments ) {
    using namespace fostlib::logging;
    scoped_sink< capture_copy > cc;
    debug(0);
    debug(0, 1);
    debug(0, 1, 2);
    debug(0, 1, 2, 3);
    fostlib::json data = cc();
    FSL_CHECK_EQ(data[0]["body"], fostlib::json(0));
}


FSL_TEST_FUNCTION( global_no_sink ) {
    using namespace fostlib::logging;
    fostlib::json conf;
    scoped_sink< capture_copy > cc;
    FSL_CHECK_EXCEPTION(global_sink_configuration sinks(conf), fostlib::exceptions::null&);
    fostlib::insert(conf, "sinks", fostlib::json());
    global_sink_configuration sinks(conf);
    fostlib::json data = cc();
    FSL_CHECK_EQ(data.size(), 1u);
}


namespace {
    bool ignore_messages(const fostlib::logging::message&) {
        return false;
    }
}
FSL_TEST_FUNCTION( function ) {
    using namespace fostlib::logging;
    scoped_sink_fn ignore(ignore_messages);
}


namespace {
    bool s_message_logged = false;
    class log_tests_global {
        public:
            log_tests_global(const fostlib::json &config) {
                FSL_CHECK_EQ(config["configured"], fostlib::json(true));
            }
            bool operator () ( const fostlib::logging::message &m ) const {
                s_message_logged = true;
                FSL_CHECK_EQ(m.body(), fostlib::json(
                    "Sending through to the global configuration"));
                return true;
            }
    };
    const fostlib::logging::global_sink<log_tests_global>
        c_log_tests_global("log-tests-global");
}
FSL_TEST_FUNCTION( global_with_sink ) {
    fostlib::logging::scoped_sink< capture_copy > cc;

    fostlib::json config, test_sink, invalid_sink;
    FSL_CHECK_EXCEPTION(
        fostlib::logging::global_sink_configuration gsc(config),
        fostlib::exceptions::null&);

    fostlib::insert(test_sink, "name", "log-tests-global");
    fostlib::insert(test_sink, "configuration", "configured", true);
    fostlib::push_back(config, "sinks", test_sink);

    fostlib::insert(invalid_sink, "name", "not a sink name");
    fostlib::push_back(config, "sinks", invalid_sink);

    FSL_CHECK(!s_message_logged);
    fostlib::logging::global_sink_configuration gsc(config);
    fostlib::logging::info("Sending through to the global configuration");

    fostlib::json data = cc();
    FSL_CHECK_EQ(data.size(), 2u);
    FSL_CHECK_EQ(data[0]["body"][1]["configuration"], config);
    FSL_CHECK_EQ(data[1]["body"], fostlib::json(
        "Sending through to the global configuration"));

    // Do this test at the end as we have to wait for the log message to get delivered
    FSL_CHECK(s_message_logged);
}
