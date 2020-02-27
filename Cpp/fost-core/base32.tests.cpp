/**
    Copyright 2018-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <fost/base32>
#include <fost/unicode>
#include <fost/test>


using namespace f5::literals;


FSL_TEST_SUITE(base32);


FSL_TEST_FUNCTION(bytes_0) {
    const auto dec =
            fostlib::detail::decode_b32_5bytes(fostlib::base32_alphabet, ""_l);
    FSL_CHECK_EQ(dec.second, 0u);
}


FSL_TEST_FUNCTION(bytes_1) {
    const std::array<f5::byte, 1> a{{f5::byte(0xff)}};
    FSL_CHECK_EQ(fostlib::coerce<fostlib::base32_string>(a), "74======");
    const auto dec = fostlib::detail::decode_b32_5bytes(
            fostlib::base32_alphabet, "74======"_l);
    FSL_CHECK_EQ(dec.second, 1u);
    FSL_CHECK_EQ(dec.first[0], 0xff);
}


FSL_TEST_FUNCTION(bytes_2) {
    const std::array<f5::byte, 2> a{{f5::byte(0xff), f5::byte(0xff)}};
    FSL_CHECK_EQ(fostlib::coerce<fostlib::base32_string>(a), "777Q====");
    const auto dec = fostlib::detail::decode_b32_5bytes(
            fostlib::base32_alphabet, "777Q===="_l);
    FSL_CHECK_EQ(dec.second, 2u);
    FSL_CHECK_EQ(dec.first[0], 0xff);
    FSL_CHECK_EQ(dec.first[1], 0xff);
}


FSL_TEST_FUNCTION(bytes_3) {
    const std::array<f5::byte, 3> a{
            {f5::byte(0xff), f5::byte(0xff), f5::byte(0xff)}};
    FSL_CHECK_EQ(fostlib::coerce<fostlib::base32_string>(a), "77776===");
    const auto dec = fostlib::detail::decode_b32_5bytes(
            fostlib::base32_alphabet, "77776==="_l);
    FSL_CHECK_EQ(dec.second, 3u);
    FSL_CHECK_EQ(dec.first[0], 0xff);
    FSL_CHECK_EQ(dec.first[1], 0xff);
    FSL_CHECK_EQ(dec.first[2], 0xff);
}


FSL_TEST_FUNCTION(bytes_4) {
    const std::array<f5::byte, 4> a{
            {f5::byte(0xff), f5::byte(0xff), f5::byte(0xff), f5::byte(0xff)}};
    FSL_CHECK_EQ(fostlib::coerce<fostlib::base32_string>(a), "777777Y=");
    const auto dec = fostlib::detail::decode_b32_5bytes(
            fostlib::base32_alphabet, "777777Y="_l);
    FSL_CHECK_EQ(dec.second, 4u);
    FSL_CHECK_EQ(dec.first[0], 0xff);
    FSL_CHECK_EQ(dec.first[1], 0xff);
    FSL_CHECK_EQ(dec.first[2], 0xff);
    FSL_CHECK_EQ(dec.first[3], 0xff);
}


FSL_TEST_FUNCTION(bytes_5) {
    const std::array<f5::byte, 5> a{{f5::byte(0xff), f5::byte(0xff),
                                     f5::byte(0xff), f5::byte(0xff),
                                     f5::byte(0xff)}};
    FSL_CHECK_EQ(fostlib::coerce<fostlib::base32_string>(a), "77777777");
    const auto dec = fostlib::detail::decode_b32_5bytes(
            fostlib::base32_alphabet, "77777777"_l);
    FSL_CHECK_EQ(dec.second, 5u);
    FSL_CHECK_EQ(dec.first[0], 0xff);
    FSL_CHECK_EQ(dec.first[1], 0xff);
    FSL_CHECK_EQ(dec.first[2], 0xff);
    FSL_CHECK_EQ(dec.first[3], 0xff);
    FSL_CHECK_EQ(dec.first[4], 0xff);
}


FSL_TEST_FUNCTION(bytes_long) {
    f5::lstring const foobar = "foobar";
    const auto b32hex = fostlib::coerce<fostlib::base32hex_string>(foobar);
    FSL_CHECK_EQ(b32hex, "CPNMUOJ1E8======");
    const auto vec = fostlib::coerce<std::vector<unsigned char>>(b32hex);
    FSL_CHECK_EQ(
            foobar, fostlib::coerce<fostlib::utf8_string>(vec).underlying());
}
