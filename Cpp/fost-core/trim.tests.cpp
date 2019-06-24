#include <fost/log>
#include <fost/test>
#include <fost/unicode>
#include <fost/trim.hpp>


FSL_TEST_SUITE(trim);


FSL_TEST_FUNCTION(empty) {
    auto s = f5::u8view("");
    auto actual = fostlib::trim(s);
    auto expected = f5::u8view("");
    FSL_CHECK_EQ(actual, expected);
}

FSL_TEST_FUNCTION(whitespace_leading) {
    auto s = f5::u8view("   string");
    auto actual = fostlib::trim(s);
    auto expected = f5::u8view("string");
    FSL_CHECK_EQ(actual, expected);
}

FSL_TEST_FUNCTION(whitespace_trailing) {
    auto s = f5::u8view("string   ");
    auto actual = fostlib::trim(s);
    auto expected = f5::u8view("string");
    FSL_CHECK_EQ(actual, expected); 
}

FSL_TEST_FUNCTION(whitespace_leading_trailing) {
    auto s = f5::u8view("   string   ");
    auto actual = fostlib::trim(s);
    auto expected = f5::u8view("string");
    FSL_CHECK_EQ(actual, expected);
}

FSL_TEST_FUNCTION(trim_only_leading_trailing_whitespaces){
    auto s = f5::u8view("   str ing   ");
    auto actual = fostlib::trim(s);
    auto expected = f5::u8view("str ing");
    FSL_CHECK_EQ(actual, expected);
}

FSL_TEST_FUNCTION(trim_nonbreaking_whitespace) {
    f5::u8view s = f5::u8view("\t string \n");
    f5::u8view res = fostlib::trim(s);
    f5::u8view expected = f5::u8view("string");
    FSL_CHECK_EQ(expected, res);
}

FSL_TEST_FUNCTION(unicode_space_safe) {
    auto s = f5::u8view(" string　 ");
    auto actual = fostlib::trim(s);
    auto expected = f5::u8view("string");
    FSL_CHECK_EQ(actual, expected);
}