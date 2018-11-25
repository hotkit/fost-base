/*
    Copyright 2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <boost/asio.hpp>


FSL_TEST_SUITE(exceptions);


FSL_TEST_FUNCTION(not_implemented) {
    FSL_CHECK_EXCEPTION(
            throw fostlib::exceptions::not_implemented(
                    "Not implemented exception thrown"),
            fostlib::exceptions::not_implemented &);
    FSL_CHECK_EXCEPTION(
            throw fostlib::exceptions::not_implemented(
                    "test_func", boost::asio::error::eof),
            fostlib::exceptions::not_implemented &);
    FSL_CHECK_EXCEPTION(
            throw fostlib::exceptions::not_implemented(
                    "test_func", boost::asio::error::eof, "Check EOF throw"),
            fostlib::exceptions::not_implemented &);
}
