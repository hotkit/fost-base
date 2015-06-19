/*
    Copyright 2010-2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/log>
#include <fost/insert>

#include <atomic>


using namespace fostlib;


namespace {
    const module c_module(c_fost_base_core, __FILE__);
}


FSL_TEST_SUITE( log );


FSL_TEST_FUNCTION( message ) {
    fostlib::nliteral test_level = "test-level";
    const fostlib::log::message m1(c_module, 12, test_level, json(true));
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
    const fostlib::log::message m2(1234, test_level2, json(false));
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

            bool operator () (const fostlib::log::message &m) {
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
    fostlib::log::scoped_sink< capture_copy > cc1, cc2(false);
    fostlib::log::info(c_module, "Log message");
    fostlib::json d1 = cc1(), d2 = cc2();
    FSL_CHECK_EQ(d1.size(), 0u);
    FSL_CHECK_EQ(d2.size(), 1u);
    FSL_CHECK_EQ(d2[0]["body"], fostlib::json("Log message"));
}
FSL_TEST_FUNCTION( log ) {
    using namespace fostlib::log;
    fostlib::log::scoped_sink< capture_copy > cc;
    fostlib::log::debug(c_module, "Example debug message -- please ignore");
    fostlib::log::info(c_module, "Example info message -- please ignore");
    fostlib::log::warning(c_module, "Example warning message -- please ignore");
    fostlib::log::error(c_module, "Example error message -- please ignore");
    fostlib::log::critical(c_module, "Example critical message -- please ignore");
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
    using namespace fostlib::log;
    scoped_sink< capture_copy > cc;
    debug(c_module, "Example debug message -- please ignore");
    info(c_module, "Example info message -- please ignore");
    warning(c_module, "Example warning message -- please ignore");
    error(c_module, "Example error message -- please ignore");
    critical(c_module, "Example critical message -- please ignore");
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


FSL_TEST_FUNCTION( log_dsl ) {
    using namespace fostlib::log;
    scoped_sink< capture_copy > cc;
    debug(c_module)
        ("key", "value")
        ("second-key", "part-a",  true)
        ("second-key", "part-b", false)
        ("third-key", fostlib::json());
    debug(c_module)
        ("goodbye", "country")
        ("hello", "nightclub");
    fostlib::json data = cc();

    FSL_CHECK(!data[0].has_key("module"));
    FSL_CHECK_EQ(data[0]["body"]["key"], fostlib::json("value"));
    FSL_CHECK_EQ(data[0]["body"]["second-key"]["part-a"], fostlib::json(true));
    FSL_CHECK_EQ(data[0]["body"]["second-key"]["part-b"], fostlib::json(false));
    FSL_CHECK_EQ(data[0]["body"]["third-key"], fostlib::json());

    FSL_CHECK(data[1].has_key("module"));
}


FSL_TEST_FUNCTION( direct_with_nullables ) {
    using namespace fostlib::log;
    scoped_sink< capture_copy > cc;
    nullable<string> empty;
    debug(c_module, empty);
    fostlib::json data = cc();
    FSL_CHECK_EQ(data[0]["body"], fostlib::json());
}


FSL_TEST_FUNCTION( direct_with_multiple_arguments ) {
    using namespace fostlib::log;
    scoped_sink< capture_copy > cc;
    debug(c_module, 0);
    debug(c_module, 0, 1);
    debug(c_module, 0, 1, 2);
    debug(c_module, 0, 1, 2, 3);
    fostlib::json data = cc();
    FSL_CHECK_EQ(data[0]["body"], fostlib::json(0));
}


FSL_TEST_FUNCTION( global_no_sink ) {
    using namespace fostlib::log;
    fostlib::json conf;
    scoped_sink< capture_copy > cc;
    FSL_CHECK_EXCEPTION(global_sink_configuration sinks(conf), fostlib::exceptions::null&);
    fostlib::insert(conf, "sinks", fostlib::json());
    global_sink_configuration sinks(conf);
    fostlib::json data = cc();
    FSL_CHECK_EQ(data.size(), 1u);
}


namespace {
    bool ignore_messages(const fostlib::log::message&) {
        return false;
    }
}
FSL_TEST_FUNCTION( function ) {
    using namespace fostlib::log;
    scoped_sink_fn ignore(ignore_messages);
}


namespace {
    boost::mutex test_has_run_mutex;
    bool constructor_entered(false), message_seen(false);
    class log_tests_global {
        public:
            log_tests_global(const fostlib::json &config) {
                boost::mutex::scoped_lock lock(test_has_run_mutex);
                constructor_entered = true;
                FSL_CHECK_EQ(config["configured"], fostlib::json(true));
            }
            bool operator () ( const fostlib::log::message &m ) {
                boost::mutex::scoped_lock lock(test_has_run_mutex);
                message_seen = true;
                FSL_CHECK_EQ(m.body(), fostlib::json(
                    "Sending through to the global configuration"));
                return true;
            }
    };
    const fostlib::log::global_sink<log_tests_global>
        c_log_tests_global("log-tests-global");
}
FSL_TEST_FUNCTION( global_with_sink ) {
    fostlib::log::scoped_sink< capture_copy > cc;

    fostlib::json config, test_sink, invalid_sink;
    FSL_CHECK_EXCEPTION(
        fostlib::log::global_sink_configuration gsc(config),
        fostlib::exceptions::null&);

    fostlib::insert(test_sink, "name", "log-tests-global");
    fostlib::insert(test_sink, "configuration", "configured", true);
    fostlib::push_back(config, "sinks", test_sink);

    fostlib::insert(invalid_sink, "name", "not a sink name");
    fostlib::push_back(config, "sinks", invalid_sink);

    fostlib::log::global_sink_configuration gsc(config);
    fostlib::log::info(c_module, "Sending through to the global configuration");

    fostlib::log::flush();
    {
        boost::mutex::scoped_lock lock(test_has_run_mutex);
        FSL_CHECK(constructor_entered);
        FSL_CHECK(message_seen);
    }

    fostlib::json data = cc();
    FSL_CHECK_EQ(data.size(), 2u);
    FSL_CHECK_EQ(data[0]["body"][1]["configuration"], config);
    FSL_CHECK_EQ(data[1]["body"], fostlib::json(
        "Sending through to the global configuration"));
}


namespace {
    std::atomic<int> g_messages_seen;
    class count_log_messages {
        public:
            count_log_messages(const fostlib::json &config) {
            }
            bool operator () ( const fostlib::log::message &m ) const {
                ++g_messages_seen;
                return false;
            }
    };
    const fostlib::log::global_sink<count_log_messages>
        c_count_log_messages("log-message-count");
}
FSL_TEST_FUNCTION( large_number_of_log_messages ) {
    fostlib::json config, counter_sink;
    fostlib::insert(counter_sink, "name", "log-message-count");
    fostlib::insert(counter_sink, "configuration", fostlib::json());
    fostlib::push_back(config, "sinks", counter_sink);
    fostlib::log::global_sink_configuration gsc(config);

    for ( std::size_t c(0); c < 1000; ++c ) {
        fostlib::log::debug(c_module, c);
    }
   fostlib:: log::flush();

    // Our 1000 messages + one sink start up message
    FSL_CHECK_EQ(g_messages_seen.load(), 1001);
}

