/*
    Copyright 2013, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"

#include <fost/exception/file_error.hpp>
#include <fost/progress.hpp>


FSL_TEST_SUITE( progress );


FSL_TEST_FUNCTION( progress_in_for_loop ) {
    const std::size_t upto = 1000;
    std::size_t sum = 0;
    for ( fostlib::progress pos(fostlib::json(), upto);
            !pos.is_complete(); ++pos )
        sum += pos.current();
    // Started at zero so only summed the first 999 integers
    FSL_CHECK_EQ(sum, 499500u);
}


FSL_TEST_FUNCTION( meter_for_loop ) {
    fostlib::meter current;
    // Appear complete before the progress instance is created
    FSL_CHECK(current.is_complete());
    fostlib::progress pos(fostlib::json(), 499500);
    // Now we know there is work left to do so not complete
    FSL_CHECK(!current.is_complete());
    for ( std::size_t number = 0; !pos.is_complete(); ++number )
        pos += number;
    FSL_CHECK_EQ(pos.current(), 499500u);
    // Now the work is done so we're complete again
    FSL_CHECK(current.is_complete());
}


namespace {
    std::size_t do_work(fostlib::meter &tracker) {
        std::size_t number = 0;
        for ( fostlib::progress pos(fostlib::json(), 499500);
                !pos.is_complete(); ++number )
            pos += number;
        return number;
    }
}
FSL_TEST_FUNCTION( meter_in_same_thread ) {
    fostlib::meter current;
    do_work(current);
    FSL_CHECK(current.is_complete());
}


FSL_TEST_FUNCTION( file_processing_progress ) {
    FSL_CHECK_EXCEPTION(
        fostlib::progress p1(boost::filesystem::wpath(L"Not-a-file.txt")),
        fostlib::exceptions::file_error&);

    fostlib::progress p2(boost::filesystem::wpath(
        L"../fost-base/LICENSE_1_0.txt"));
#ifdef FOST_OS_WINDOWS
    // On Windows the line endings are CRLF rather than LF
    FSL_CHECK_EQ(p2.total(), 1361u);
#else
    FSL_CHECK_EQ(p2.total(), 1338u);
#endif
}

