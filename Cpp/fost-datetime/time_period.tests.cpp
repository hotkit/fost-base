/*
    Copyright 2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/timer>
#include <fost/test>


using namespace std::literals::chrono_literals;


FSL_TEST_SUITE(time_period);


FSL_TEST_FUNCTION(basic) {
    fostlib::time_profile<std::chrono::milliseconds> ms(20ms, 1.1, 2);
    FSL_CHECK_EQ(ms.max_time().count(), 42);
    FSL_CHECK_EQ(ms.size(), 2u);
    ms.record(10ms);
    FSL_CHECK_EQ(ms[0], 1u);
    FSL_CHECK_EQ(ms[1], 0u);
    FSL_CHECK_EQ(ms.overflow(), 0u);
    ms.record(30ms);
    FSL_CHECK_EQ(ms[0], 1u);
    FSL_CHECK_EQ(ms[1], 1u);
    FSL_CHECK_EQ(ms.overflow(), 0u);
    ms.record(60ms);
    FSL_CHECK_EQ(ms[0], 1u);
    FSL_CHECK_EQ(ms[1], 1u);
    FSL_CHECK_EQ(ms.overflow(), 1u);
    ms.record(20ms);
    FSL_CHECK_EQ(ms[0], 2u);
    FSL_CHECK_EQ(ms[1], 1u);
    FSL_CHECK_EQ(ms.overflow(), 1u);

    auto js = fostlib::coerce<fostlib::json>(ms);
    FSL_CHECK_EQ(js.size(), 3u);
    FSL_CHECK_EQ(js[0]["time"], fostlib::json(20));
    FSL_CHECK_EQ(js[0]["count"], fostlib::json(2));
    FSL_CHECK_EQ(js[1]["time"], fostlib::json(42));
    FSL_CHECK_EQ(js[1]["count"], fostlib::json(1));
    FSL_CHECK_EQ(js[2], fostlib::json(1));
}

