/**
    Copyright 2008-2019, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-core-test.hpp"
#include <fost/unicode>


FSL_TEST_SUITE(string);


FSL_TEST_FUNCTION(constructors) {
    fostlib::test::default_copy_constructable<fostlib::string>();
    FSL_CHECK_EQ(fostlib::string(L"aaa"), fostlib::string("aaa"));
    FSL_CHECK_EQ(fostlib::string(3, 'a'), fostlib::string(L"aaa"));

    const wchar_t *s = L"Hello";
    FSL_CHECK_EQ(fostlib::string(L"Hello"), s);
    FSL_CHECK_EQ(fostlib::string(s), "Hello");

    fostlib::string h("hello");
    FSL_CHECK_EQ(fostlib::string(h.begin(), h.end()), h);

    std::optional<fostlib::string> ns, ns1{"Hi"}, ns2{L"Hi"}, ns3{L"\x2014"};
    FSL_CHECK(not ns);
    FSL_CHECK(ns1);
    FSL_CHECK_EQ(ns1, "Hi");
    FSL_CHECK_EQ(ns2, "Hi");
    FSL_CHECK_EQ(ns3, "\xe2\x80\x94");
}


FSL_TEST_FUNCTION(concatenation) {
    FSL_CHECK_EQ(fostlib::string("hello ") + "world", "hello world");
    FSL_CHECK_EQ(fostlib::string("hello ") + L"world", "hello world");
    FSL_CHECK_EQ("hello" + fostlib::string(" ") + "world", "hello world");
    FSL_CHECK_EQ(L"hello" + fostlib::string(" ") + L"world", "hello world");
    FSL_CHECK_EQ(fostlib::string{"Hello"} + '!', "Hello!");
    FSL_CHECK_EQ(fostlib::string{"Hello"} + L'!', "Hello!");
    fostlib::string hello{"Hello"};
    hello += L'!';
    FSL_CHECK_EQ(hello, "Hello!");
    hello += L' ';
    FSL_CHECK_EQ(hello, "Hello! ");
}


FSL_TEST_FUNCTION(comparison) {
    fostlib::string one("one"), two("two");
    FSL_CHECK_EQ(one, one);
    FSL_CHECK_EQ(one, "one");
    FSL_CHECK_EQ("one", one);
    FSL_CHECK_EQ(one, L"one");
    FSL_CHECK_EQ(L"one", one);

    FSL_CHECK_NEQ(one, two);
    FSL_CHECK_NEQ(two, one);
    FSL_CHECK_NEQ(two, "one");
    FSL_CHECK_NEQ("one", two);
    FSL_CHECK_NEQ(two, L"one");
    FSL_CHECK_NEQ(L"one", two);

    FSL_CHECK(one <= one);
    FSL_CHECK(one >= one);
    FSL_CHECK(one < two);
    FSL_CHECK(two > one);
    FSL_CHECK(one <= two);
    FSL_CHECK(two >= one);

    FSL_CHECK(one <= "one");
    FSL_CHECK(one >= "one");
    FSL_CHECK(one < "two");
    FSL_CHECK(two > "one");
    FSL_CHECK(one <= "two");
    FSL_CHECK(two >= "one");

    FSL_CHECK("one" <= one);
    FSL_CHECK("one" >= one);
    FSL_CHECK("one" < two);
    FSL_CHECK("two" > one);
    FSL_CHECK("one" <= two);
    FSL_CHECK("two" >= one);

    FSL_CHECK(one <= L"one");
    FSL_CHECK(one >= L"one");
    FSL_CHECK(one < L"two");
    FSL_CHECK(two > L"one");
    FSL_CHECK(one <= L"two");
    FSL_CHECK(two >= L"one");

    FSL_CHECK(L"one" <= one);
    FSL_CHECK(L"one" >= one);
    FSL_CHECK(L"one" < two);
    FSL_CHECK(L"two" > one);
    FSL_CHECK(L"one" <= two);
    FSL_CHECK(L"two" >= one);
}


FSL_TEST_FUNCTION(access) {
    FSL_CHECK_EQ(fostlib::string(L"Hello\x2014world!").at(1), 'e');
    FSL_CHECK_EQ(fostlib::string(L"Hello\x2014world!").at(4), 'o');
    FSL_CHECK_EQ(fostlib::string(L"Hello\x2014world!").at(5), 0x2014);
    FSL_CHECK_EQ(fostlib::string(L"Hello\x2014world!").at(6), 'w');

    FSL_CHECK_EQ(fostlib::string(L"\x5b6b").at(0), 0x5b6b);
    FSL_CHECK_EQ(fostlib::string(L"\xd834\xdd1e").at(0), 0x1d11e);
}


FSL_TEST_FUNCTION(lengths) {
    FSL_CHECK_EQ(fostlib::string(L"aaa").length(), 3u);
    FSL_CHECK_EQ(fostlib::string(L"Hello\x2014world!").length(), 12u);
    FSL_CHECK_EQ(fostlib::string(L"a\x2014x").length(), 3u);
    FSL_CHECK_EQ(fostlib::string(L"\xd834\xdd1e").length(), 1u);
    FSL_CHECK_EQ(fostlib::string(L"a\xd834\xdd1ex").length(), 3u);
    if (sizeof(wchar_t) == 2) {
        FSL_CHECK_EQ(fostlib::string(L"a\x2014x").native_length(), 3u);
        FSL_CHECK_EQ(
                fostlib::string(L"Hello\x2014world!").native_length(), 12u);
        FSL_CHECK_EQ(fostlib::string(L"\xd834\xdd1e").native_length(), 2u);
        FSL_CHECK_EQ(fostlib::string(L"a\xd834\xdd1ex").native_length(), 4u);
    } else {
        FSL_CHECK_EQ(fostlib::string(L"a\x2014x").native_length(), 5u);
        FSL_CHECK_EQ(
                fostlib::string(L"Hello\x2014world!").native_length(), 14u);
        // These two lengths do not look right
        FSL_CHECK_EQ(fostlib::string(L"\xd834\xdd1e").native_length(), 4u);
        FSL_CHECK_EQ(fostlib::string(L"a\xd834\xdd1ex").native_length(), 6u);
    }
}


FSL_TEST_FUNCTION(casts) {
    FSL_CHECK_EQ(f5::u8view{fostlib::string("aaa")}, "aaa");
}


FSL_TEST_FUNCTION(substr) {
    FSL_CHECK_EQ(fostlib::string(L"abc").substr(), L"abc");
    FSL_CHECK_EQ(fostlib::string(L"abc").substr(1, 0), L"");
    FSL_CHECK_EQ(fostlib::string(L"abc").substr(1, 1), L"b");
    FSL_CHECK_EQ(fostlib::string(L"abc").substr(0, 2), L"ab");
    FSL_CHECK_EQ(fostlib::string(L"abc").substr(0, 10), L"abc");
    FSL_CHECK_EQ(fostlib::string(L"abc").substr(4), L"");
    FSL_CHECK_EQ(fostlib::string(L"abc").substr(fostlib::string::npos), L"");

    FSL_CHECK_EQ(fostlib::string(L"a\x2014").substr(1, 1), L"\x2014");
    FSL_CHECK_EQ(fostlib::string(L"a\x2014x").substr(1, 1), L"\x2014");
    FSL_CHECK_EQ(
            fostlib::string(L"a\xd834\xdd1ex").substr(1, 1), L"\xd834\xdd1e");
}


FSL_TEST_FUNCTION(erase) {
    FSL_CHECK_EQ(fostlib::string(L"abc").erase(), L"");
    FSL_CHECK_EQ(fostlib::string(L"abc").erase(1, 0), L"abc");
    FSL_CHECK_EQ(fostlib::string(L"abc").erase(1, 1), L"ac");
    FSL_CHECK_EQ(fostlib::string(L"abc").erase(0, 2), L"c");
    FSL_CHECK_EQ(fostlib::string(L"abc").erase(0, 10), L"");
    FSL_CHECK_EQ(fostlib::string(L"abc").erase(4), L"");

    FSL_CHECK_EQ(fostlib::string(L"a\x2014").erase(1, 1), L"a");
    FSL_CHECK_EQ(fostlib::string(L"a\x2014x").erase(1, 1), L"ax");
    FSL_CHECK_EQ(fostlib::string(L"a\xd834\xdd1ex").erase(1, 1), L"ax");
}


FSL_TEST_FUNCTION(insert) {
    FSL_CHECK_EQ(fostlib::string(L"abc").insert(0, L"1"), L"1abc");
    FSL_CHECK_EQ(fostlib::string(L"abc").insert(1, L"2"), L"a2bc");
    FSL_CHECK_EQ(fostlib::string(L"abc").insert(2, L"3"), L"ab3c");
    FSL_CHECK_EQ(fostlib::string(L"abc").insert(3, L"4"), L"abc4");
    FSL_CHECK_EQ(fostlib::string(L"abc").insert(4, L"5"), L"abc5");
}


FSL_TEST_FUNCTION(find) {
    FSL_CHECK_EQ(fostlib::string(L"Hello\x2014world!").find('w'), 6u);
    FSL_CHECK_EQ(fostlib::string(L"Hello\x2014world!").find('w', 4), 6u);
    FSL_CHECK_EQ(fostlib::string(L"Hello\x2014world!").find('w', 6), 6u);
    FSL_CHECK_EQ(fostlib::string(L"Hello\x2014world!").find(L"world"), 6u);
    FSL_CHECK_EQ(fostlib::string(L"Hello\x2014world!").find(L"world", 4), 6u);
    FSL_CHECK_EQ(fostlib::string(L"Hello\x2014world!").find(L"world", 6), 6u);
    FSL_CHECK_EQ(
            fostlib::string(L"Hello\x2014world!")
                    .find(L"world", fostlib::string::npos),
            fostlib::string::npos);
    FSL_CHECK_EQ(
            fostlib::string(L"Hello\x2014world!").find(L"wild"),
            fostlib::string::npos);
    FSL_CHECK_EQ(
            fostlib::string(L"Hello\x2014world!").find(L"wild", 4),
            fostlib::string::npos);
    FSL_CHECK_EQ(
            fostlib::string(L"Hello\x2014world!").find(L"wild", 6),
            fostlib::string::npos);
    FSL_CHECK_EQ(fostlib::string(L"Hello\x2014world!").find(0x2014), 5u);

    FSL_CHECK_EQ(
            fostlib::string(L"Hello\x2014world!").find_first_of(0x2014), 5u);
    FSL_CHECK_EQ(
            fostlib::string(L"Hello\x2014world!").find_first_of(L"xe"), 1u);
    FSL_CHECK_EQ(
            fostlib::string(L"Hello\x2014world!").find_first_not_of(L"He"), 2u);

    FSL_CHECK_EQ(
            fostlib::string(L"Hello\xd834\xdd1eworld!").find_first_of(0x1d11e),
            5u);
    FSL_CHECK_EQ(
            fostlib::string(L"Hello\xd834\xdd1eworld!").find_first_of(L"xe"),
            1u);
    FSL_CHECK_EQ(
            fostlib::string(L"Hello\xd834\xdd1eworld!").find_first_not_of(L"He"),
            2u);

    FSL_CHECK_EQ(fostlib::string(L"abc").find_last_not_of(L" \n\t"), 2u);

    FSL_CHECK_EQ(fostlib::string(L"abc").find(L"."), fostlib::string::npos);
}


FSL_TEST_FUNCTION(startswith_char) {
    fostlib::string test("a test string");
    FSL_CHECK(test.startswith('a'));
    FSL_CHECK(!test.startswith('g'));
}


FSL_TEST_FUNCTION(startswith_nliteral) {
    fostlib::string test("a test string");
    FSL_CHECK(test.startswith("a"));
    FSL_CHECK(test.startswith("a "));
    FSL_CHECK(test.startswith("a test"));
    FSL_CHECK(!test.startswith("test"));
    FSL_CHECK(test.startswith(""));
    FSL_CHECK(!test.startswith("a test string which is long"));
}


FSL_TEST_FUNCTION(startswith_wliteral) {
    fostlib::string test("a test string");
    FSL_CHECK(test.startswith(L"a"));
    FSL_CHECK(test.startswith(L"a "));
    FSL_CHECK(test.startswith(L"a test"));
    FSL_CHECK(!test.startswith(L"test"));
    FSL_CHECK(test.startswith(L""));
    FSL_CHECK(!test.startswith(L"a test string which is long"));
}


FSL_TEST_FUNCTION(startswith_string) {
    fostlib::string test("a test string");
    FSL_CHECK(test.startswith('a'));
    FSL_CHECK(!test.startswith('g'));
    FSL_CHECK(test.startswith(fostlib::string("a")));
    FSL_CHECK(test.startswith(fostlib::string("a ")));
    FSL_CHECK(test.startswith(fostlib::string("a test")));
    FSL_CHECK(!test.startswith(fostlib::string("test")));
    FSL_CHECK(test.startswith(fostlib::string("")));
    FSL_CHECK(!test.startswith(fostlib::string("a test string which is long")));
}


FSL_TEST_FUNCTION(endswith_string) {
    fostlib::string test("a test string");
    FSL_CHECK(test.endswith("g"));
    FSL_CHECK(test.endswith("string"));
    FSL_CHECK(test.endswith("t string"));
    FSL_CHECK(!test.endswith("test"));
    FSL_CHECK(!test.endswith("t strin"));
    FSL_CHECK(!test.endswith("not a test string"));
}


FSL_TEST_FUNCTION(endswith_char) {
    fostlib::string test("a test string");
    FSL_CHECK(!test.endswith('a'));
    FSL_CHECK(test.endswith('g'));
}


FSL_TEST_FUNCTION(replace) {
    FSL_CHECK_EQ(
            fostlib::string(L"abc").replace(1, 1, fostlib::string(L"xyz", 1, 1)),
            L"ayc");
}


FSL_TEST_FUNCTION(utility_trim) {
    FSL_CHECK_NULL(fostlib::trim(L"   "));

    FSL_CHECK_EQ(fostlib::trim(L"abc"), "abc");
    FSL_CHECK_EQ(fostlib::trim(L"  abc"), "abc");
    FSL_CHECK_EQ(fostlib::trim(L"  abc "), "abc");

    FSL_CHECK_EQ(fostlib::trim(L"Hello\x2014world!"), L"Hello\x2014world!");
}


FSL_TEST_FUNCTION(utility_partition) {
    fostlib::string abc{"abc"};
    FSL_CHECK_EQ(fostlib::partition(abc, ".").first, "abc");
    FSL_CHECK_NULL(fostlib::partition(abc, ".").second);
    FSL_CHECK_EQ(fostlib::partition(abc, "b").first, "a");
    FSL_CHECK_EQ(fostlib::partition(abc, "b").second.value(), "c");
    FSL_CHECK_EQ(fostlib::partition(abc, "c").first, "ab");
    FSL_CHECK_NULL(fostlib::partition(abc, "c").second);
}


FSL_TEST_FUNCTION(utility_split) {
    FSL_CHECK_EQ(fostlib::split(L"abc", L".")[0], L"abc");
    FSL_CHECK_EQ(fostlib::split(L"abc", L"a")[0], L"bc");
    FSL_CHECK_EQ(fostlib::split(L"abc", L"b")[0], L"a");
    FSL_CHECK_EQ(fostlib::split(L"abc", L"b")[1], L"c");
    FSL_CHECK_EQ(fostlib::split(L"abc", L"c")[0], L"ab");

    FSL_CHECK_EQ(fostlib::split(L"abc", L"ab")[0], L"c");
    FSL_CHECK_EQ(fostlib::split(L"abc", L"bc")[0], L"a");

    FSL_CHECK(fostlib::split(L"abc", L"abc").empty());
}


FSL_TEST_FUNCTION(utility_crack) {
    FSL_CHECK_EQ(fostlib::crack("ab='asdf'", "='", "'").first, "ab");
    FSL_CHECK_EQ(fostlib::crack("ab='asdf'", "='", "'").second.value(), "asdf");
    FSL_CHECK_EQ(
            fostlib::crack("  ab='asdf'  ", "='", "'").second.value(), "asdf");
    FSL_CHECK_EQ(
            fostlib::crack("ab<as<x>>'", "<", ">").second.value(), "as<x>");
    // FSL_CHECK_EQ( fostlib::crack( L"boundary=\"=_some_boundary_=\"", L"=\"",
    // L"\"" ).second.value(), L"=_some_boundary_=" );
}
