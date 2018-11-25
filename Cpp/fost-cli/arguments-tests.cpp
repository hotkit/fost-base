/*
    Copyright 2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/main>
#include <fost/test>


FSL_TEST_SUITE(arguments);


#define TEST_ARG(args, count) \
    FSL_CHECK_EQ( \
            fostlib::arguments( \
                    sizeof(args) / sizeof(fostlib::native_char *), args) \
                    .size(), \
            count)
FSL_TEST_FUNCTION(argument_count) {
    /*
    const fostlib::native_char * argv1a[] = { "executable 1a" };
    const fostlib::native_char * argv1b[] = { "executable 1b", "-x" };
    const fostlib::native_char * argv2a[] = { "executable 2a", "arg2" };
    const fostlib::native_char * argv2b[] = {
        "executable 2b", "-something", "value", "arg2"
    };

    TEST_ARG( argv1a, 1u );
    TEST_ARG( argv1b, 1u );
    TEST_ARG( argv2a, 2u );
    TEST_ARG( argv2b, 2u );
    */
}
