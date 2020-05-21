/**
    Copyright 2008-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-core-test.hpp"

#include <fost/core>
#include <fost/exception/not_a_number.hpp>
#include <fost/exception/parse_error.hpp>


FSL_TEST_SUITE(coercion);


template<typename T>
using out_of_range = fostlib::exceptions::out_of_range<T, int64_t>;
FSL_TEST_FUNCTION(integral) {
    FSL_CHECK_EQ(int8_t{34}, fostlib::coerce<int8_t>(int64_t{34}));
    FSL_CHECK_EQ(int8_t{-34}, fostlib::coerce<int8_t>(int64_t{-34}));
    FSL_CHECK_EXCEPTION(
            fostlib::coerce<int8_t>(int64_t{3434}), out_of_range<int8_t> &);
    FSL_CHECK_EXCEPTION(
            fostlib::coerce<uint8_t>(int64_t{-34}), out_of_range<uint8_t> &);
    FSL_CHECK_EXCEPTION(
            fostlib::coerce<uint64_t>(int64_t{-34}), out_of_range<uint64_t> &);
}


FSL_TEST_FUNCTION(int64_t) {
    FSL_CHECK_EXCEPTION(
            fostlib::coerce<int64_t>(fostlib::string()),
            fostlib::exceptions::parse_error &);
    FSL_CHECK_EQ(fostlib::coerce<int64_t>(fostlib::string("0")), 0);
    FSL_CHECK_EQ(fostlib::coerce<int64_t>(fostlib::string("123")), 123);
    FSL_CHECK_EQ(fostlib::coerce<int64_t>(fostlib::string("1234")), 1234);
    FSL_CHECK_EQ(fostlib::coerce<int64_t>(fostlib::string("12345")), 12345);
    FSL_CHECK_EQ(fostlib::coerce<int64_t>(fostlib::string("123456")), 123456);
    FSL_CHECK_EQ(fostlib::coerce<int64_t>(fostlib::string("1234567")), 1234567);
    FSL_CHECK_EQ(
            fostlib::coerce<int64_t>(fostlib::string("17179869184")),
            int64_t(1) << 34);
    FSL_CHECK_EQ(
            fostlib::coerce<int64_t>(fostlib::string("18014398509481984")),
            int64_t(1) << 54);
    FSL_CHECK_EQ(
            fostlib::coerce<int64_t>(fostlib::string("4611686018427387904")),
            int64_t(1) << 62);
    FSL_CHECK_EQ(fostlib::coerce<int64_t>(fostlib::string("-123")), -123);
}


FSL_TEST_FUNCTION(uint16_t) {
    FSL_CHECK_EXCEPTION(
            fostlib::coerce<uint16_t>(fostlib::string()),
            fostlib::exceptions::parse_error &);
    FSL_CHECK_EQ(fostlib::coerce<uint16_t>(fostlib::string("0")), 0);
    FSL_CHECK_EQ(fostlib::coerce<uint16_t>(fostlib::string("123")), 123);
    FSL_CHECK_EQ(fostlib::coerce<uint16_t>(fostlib::string("1234")), 1234);
    FSL_CHECK_EQ(fostlib::coerce<uint16_t>(fostlib::string("12345")), 12345);
    FSL_CHECK_EXCEPTION(
            fostlib::coerce<uint16_t>(fostlib::string("123456")),
            fostlib::exceptions::parse_error &);
    FSL_CHECK_EXCEPTION(
            fostlib::coerce<uint16_t>(fostlib::string("-123")),
            fostlib::exceptions::parse_error &);
}


FSL_TEST_FUNCTION(double) {
    FSL_CHECK_EQ(fostlib::coerce<double>(fostlib::string("1")), 1.);
    FSL_CHECK_EQ(fostlib::coerce<double>(fostlib::string("1.5")), 1.5);
    FSL_CHECK_EQ(fostlib::coerce<double>(fostlib::string(" 1")), 1.);
    FSL_CHECK_EQ(fostlib::coerce<double>(fostlib::string(" 1.5")), 1.5);
    FSL_CHECK_EQ(fostlib::coerce<double>(fostlib::string("1 ")), 1.);
    FSL_CHECK_EQ(fostlib::coerce<double>(fostlib::string("1.5 ")), 1.5);
}


FSL_TEST_FUNCTION(string) {
    BOOST_STATIC_ASSERT(!boost::is_integral<fostlib::string>::value);
    BOOST_STATIC_ASSERT(!boost::is_integral<char[7]>::value);
    BOOST_STATIC_ASSERT(boost::is_integral<char>::value);

    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::string>(fostlib::null), fostlib::string());

    FSL_CHECK_EQ(fostlib::coerce<fostlib::string>(10), "10");
    FSL_CHECK_EQ(fostlib::coerce<fostlib::string>(3.141), "3.141");
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::string>(3.141f), "3.1410000324249268");

    FSL_CHECK_EQ(fostlib::coerce<fostlib::string>("3.141f"), "3.141f");
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::string>("Hello world"), "Hello world");

    FSL_CHECK_EQ(fostlib::coerce<fostlib::string>(1 << 14), "16384");
    FSL_CHECK_EQ(fostlib::coerce<fostlib::string>(1 << 24), "16777216");
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::string>(int64_t(1) << 34), "17179869184");
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::string>(int64_t(1) << 44),
            "17592186044416");
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::string>(int64_t(1) << 54),
            "18014398509481984");
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::string>(int64_t(1) << 62),
            "4611686018427387904");
    FSL_CHECK_EQ(fostlib::coerce<fostlib::string>(-(1 << 14)), "-16384");
    FSL_CHECK_EQ(fostlib::coerce<fostlib::string>(-(1 << 24)), "-16777216");
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::string>(-(int64_t(1) << 34)),
            "-17179869184");
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::string>(-(int64_t(1) << 44)),
            "-17592186044416");
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::string>(-(int64_t(1) << 54)),
            "-18014398509481984");
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::string>(-(int64_t(1) << 62)),
            "-4611686018427387904");

    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::string>(uint64_t(1) << 34), "17179869184");
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::string>(uint64_t(1) << 44),
            "17592186044416");
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::string>(uint64_t(1) << 54),
            "18014398509481984");
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::string>(uint64_t(1) << 63),
            "9223372036854775808");

    FSL_CHECK_EQ(
            fostlib::coerce<uint16_t>(fostlib::string("123")), uint16_t(123));
    FSL_CHECK_EQ(
            fostlib::coerce<int32_t>(fostlib::string("-123")), int32_t(-123));
    FSL_CHECK_EQ(
            fostlib::coerce<int64_t>(fostlib::string("9223372036854775807")),
            int64_t(9223372036854775807ll));
}


FSL_TEST_FUNCTION(convert_from_arabic) {
    std::vector<unsigned char> data(2);
    data[0] = 0xd8;
    data[1] = 0xa7;
    FSL_CHECK_EQ(
            fostlib::string(u"\x0627"),
            fostlib::coerce<fostlib::string>(
                    fostlib::coerce<fostlib::utf8_string>(data)));
}


FSL_TEST_FUNCTION(exception_json) {
    fostlib::json expected;
    fostlib::insert(expected, "exception", "Feature not implemented");
    fostlib::insert(expected, "data", "function", __PRETTY_FUNCTION__);
    fostlib::insert(expected, "data", "message", "Test exception");
    try {
        throw fostlib::exceptions::not_implemented(
                __PRETTY_FUNCTION__, "Test exception");
    } catch (fostlib::exceptions::exception &e) {
        FSL_CHECK_EQ(fostlib::coerce<fostlib::json>(e), expected);
    }
}


FSL_TEST_FUNCTION(exception_string) {
    try {
        throw fostlib::exceptions::not_implemented("Test exception");
    } catch (fostlib::exceptions::exception &e) {
        FSL_CHECK_EQ(
                fostlib::coerce<fostlib::string>(e),
                "Feature not implemented\nData: {\n    \"function\" : \"Test "
                "exception\"\n}\n");
    }
    try {
        throw fostlib::exceptions::not_implemented("Test exception");
    } catch (fostlib::exceptions::not_implemented &e) {
        FSL_CHECK_EQ(
                fostlib::coerce<fostlib::string>(e),
                "Feature not implemented\nData: {\n    \"function\" : \"Test "
                "exception\"\n}\n");
    }
}


FSL_TEST_FUNCTION(nullables) {
    fostlib::nullable<int> empty, one(1);
    FSL_CHECK_EXCEPTION(
            fostlib::coerce<fostlib::string>(empty),
            fostlib::exceptions::null &);
    FSL_CHECK_EQ(fostlib::coerce<fostlib::string>(one), "1");
}
