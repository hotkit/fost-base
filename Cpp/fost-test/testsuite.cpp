/**
    Copyright 2007-2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-test.hpp"
#include <fost/test.hpp>

#include <fost/insert>
#include <fost/log>
#include <fost/timer>

#include "boost/date_time/posix_time/posix_time.hpp"


using namespace fostlib;


const module fostlib::c_fost_base_test(c_fost_base, "test");


namespace {


    using suite_t = threadsafe_store<fostlib::reference_ptr<const test::suite>>;
    suite_t &g_suites() {
        static suite_t s;
        return s;
    }


    setting< bool > c_verbose(
        L"fost-test/testsuite.cpp",
        L"Tests", L"Display test names", false,
        true );
    setting< bool > c_continue(
        L"fost-test/testsuite.cpp",
        L"Tests", L"Continue after error", true,
        true );
    setting< double > c_warning_test_duration(
        L"fost-test/testsuite.cpp",
        L"Tests", L"Warning test duration", 10.0,
        true );


}


/**
    ## fostlib::test::test
*/


fostlib::test::test::test( const suite &s, const fostlib::string &n ) {
    s.add( n, this );
}


void fostlib::test::test::execute() const {
    execute_inner();
}


/**
    ## fostlib::test::suite
*/


fostlib::test::suite::suite( const fostlib::string &name )
: m_name( name ) {
    g_suites().add( m_name, this );
}


void fostlib::test::suite::add( const fostlib::string &n, const fostlib::test::test *t ) const {
    (*g_suites().find( m_name ).begin())->m_tests.add( n, t );
}


namespace {
    std::mutex key;
    fostlib::json messages;

    fostlib::json clear_messages() {
        fostlib:: log::flush();
        std::unique_lock<std::mutex> lock(key);
        return std::exchange(messages, fostlib::json());
    }

    class capture_copy {
    public:
        capture_copy(const fostlib::json) {
        }
        bool operator () (const fostlib::log::message &m) {
            using namespace fostlib;
            std::unique_lock<std::mutex> lock(key);
            push_back(messages, coerce<json>(m));
            return false;
        }
    };
    const fostlib::log::global_sink<capture_copy> c_cc("test.capture-copy");

    bool loop(ostream *op) {
        bool exception{false};
        fostlib::json log_conf;
        fostlib::insert(log_conf, "sinks", 0, "name", "test.capture-copy");
        fostlib::insert(log_conf, "sinks", 0, "configuration", fostlib::json());
        for ( auto &&sn : g_suites().keys() ) {
            try {
                for ( auto &&suite : g_suites().find(sn) ) {
                    fostlib::test::suite::test_keys_type testnames(suite->test_keys());
                    for ( auto &&tn : testnames ) {
                        if ( op && c_verbose.value() )
                            *op << sn << L": " << tn << '\n';
                        auto tests(suite->tests(tn));
                        for ( auto &&test : tests ) {
                            fostlib::log::global_sink_configuration gsc(log_conf);
                            try {
                                fostlib::log::info(c_fost_base_test, "Starting test " + sn + "--" + tn);
                                const timer started;
                                test->execute();
                                const double elapsed = started.seconds();
                                if ( elapsed >c_warning_test_duration.value() )
                                    fostlib::log::warning(c_fost_base_test,
                                        "Test " + sn + "--" + tn + " took "
                                            + coerce<string>(elapsed) + "s");
                            } catch ( fostlib::exceptions::exception &e ) {
                                exception = true;
                                insert(e.data(), "test", "test", tn);
                                insert(e.data(), "test", "log", clear_messages());
                                throw;
                            } catch ( ... ) {
                                exception = true;
                                throw;
                            }
                            clear_messages();
                        }
                    }
                }
            } catch ( exceptions::exception &e ) {
                insert(e.data(), "test", "suite", sn);
                if ( op ) {
                    *op << e << std::endl;
                } else if ( not c_continue.value() ) {
                    throw;
                }
            }
        }
        return exception;
    }
}


bool fostlib::test::suite::execute() {
    return loop( NULL );
}


bool fostlib::test::suite::execute( ostream &o ) {
    return loop( &o );
}


/**
    ## fostlib::exceptions::test_failure
*/


fostlib::exceptions::test_failure::test_failure(
    const string &cond, nliteral file, int64_t line
) : exception( cond ) {
    fostlib::insert(m_data, "test", "location", "file", file);
    fostlib::insert(data(), "test", "location", "line", line);
}


const wchar_t *const fostlib::exceptions::test_failure::message() const {
    return L"Test failure";
}

