/*
    Copyright 2012-2014, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/datetime>
#include <fost/test>


FSL_TEST_SUITE(timediff);


FSL_TEST_FUNCTION(to_json_with_hours) {
    fostlib::timediff td(2, 13, 45);
    fostlib::json jtd(fostlib::coerce<fostlib::json>(td));
    FSL_CHECK_EQ(jtd["hours"], fostlib::json(2));
    FSL_CHECK_EQ(jtd["minutes"], fostlib::json(13));
    FSL_CHECK_EQ(jtd["seconds"], fostlib::json(45.0));
}


FSL_TEST_FUNCTION(to_json_without_hours) {
    fostlib::timediff td(0, 13, 45);
    fostlib::json jtd(fostlib::coerce<fostlib::json>(td));
    FSL_CHECK(!jtd.has_key("hours"));
    FSL_CHECK_EQ(jtd["minutes"], fostlib::json(13));
    FSL_CHECK_EQ(jtd["seconds"], fostlib::json(45.0));
}


FSL_TEST_FUNCTION(to_json_with_fraction) {
    fostlib::timediff td(0, 13, 45, 123456);
    fostlib::json jtd(fostlib::coerce<fostlib::json>(td));
    FSL_CHECK(!jtd.has_key("hours"));
    FSL_CHECK_EQ(jtd["minutes"], fostlib::json(13));
    FSL_CHECK_EQ(jtd["seconds"], fostlib::json(45.123456));
}


FSL_TEST_FUNCTION(from_json_hours) {
    fostlib::json td;
    fostlib::insert(td, "hours", 3);
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::timediff>(td), fostlib::timediff(3, 0, 0));
}


FSL_TEST_FUNCTION(from_json_minutes) {
    fostlib::json td;
    fostlib::insert(td, "minutes", 5);
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::timediff>(td), fostlib::timediff(0, 5, 0));
}


FSL_TEST_FUNCTION(from_json_seconds) {
    fostlib::json td;
    fostlib::insert(td, "seconds", 25.00034);
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::timediff>(td),
            fostlib::timediff(0, 0, 25, 340));
}


FSL_TEST_FUNCTION(compare) {
    fostlib::timediff s(10, 4, 45), l(11, 4, 13);
    FSL_CHECK(s < l);
    FSL_CHECK(l > s);
    FSL_CHECK(s <= l);
    FSL_CHECK(l >= s);
    FSL_CHECK(s <= s);
    FSL_CHECK(s >= s);
}


FSL_TEST_FUNCTION(maths) {
    fostlib::date d(2013, 10, 5);
    FSL_CHECK_EQ(d + fostlib::days(1), fostlib::date(2013, 10, 6));
}
