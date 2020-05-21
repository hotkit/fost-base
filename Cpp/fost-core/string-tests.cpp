/**
    Copyright 2008-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-core-test.hpp"
#include <fost/unicode>


FSL_TEST_SUITE(string);


FSL_TEST_FUNCTION(constructors) {
    fostlib::test::default_copy_constructable<fostlib::string>();
    FSL_CHECK_EQ(fostlib::string("aaa"), fostlib::string("aaa"));
    FSL_CHECK_EQ(fostlib::string(3, 'a'), fostlib::string("aaa"));

    const char *s = "Hello";
    FSL_CHECK_EQ(fostlib::string("Hello"), s);
    FSL_CHECK_EQ(fostlib::string(s), "Hello");

    fostlib::string h("hello");
    FSL_CHECK_EQ(fostlib::string(h.begin(), h.end()), h);

    std::optional<fostlib::string> ns, ns1{"Hi"}, ns2{u"Hi"}, ns3{u"\x2014"};
    FSL_CHECK(not ns);
    FSL_CHECK(ns1);
    FSL_CHECK_EQ(ns1, "Hi");
    FSL_CHECK_EQ(ns2, "Hi");
    FSL_CHECK_EQ(ns3, "\xe2\x80\x94");
}


FSL_TEST_FUNCTION(concatenation) {
    FSL_CHECK_EQ(fostlib::string("hello ") + "world", "hello world");
    FSL_CHECK_EQ("hello" + fostlib::string(" ") + "world", "hello world");
    FSL_CHECK_EQ(fostlib::string{"Hello"} + '!', "Hello!");
    FSL_CHECK_EQ(fostlib::string{"Hello"} + u'!', "Hello!");
    fostlib::string hello{"Hello"};
    hello += u'!';
    FSL_CHECK_EQ(hello, "Hello!");
    hello += u' ';
    FSL_CHECK_EQ(hello, "Hello! ");
}


FSL_TEST_FUNCTION(comparison) {
    fostlib::string one("one"), two("two");

    FSL_CHECK_EQ(one, one);
    FSL_CHECK_EQ(one, "one");
    FSL_CHECK_EQ("one", one);
    FSL_CHECK_EQ(one, u"one");
    FSL_CHECK_EQ(u"one", one);

    FSL_CHECK_NEQ(one, two);
    FSL_CHECK_NEQ(two, one);
    FSL_CHECK_NEQ(two, "one");
    FSL_CHECK_NEQ("one", two);
    FSL_CHECK_NEQ(two, u"one");
    FSL_CHECK_NEQ(u"one", two);

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
}


FSL_TEST_FUNCTION(access) {
    FSL_CHECK_EQ(fostlib::string(u"Hello\x2014world!").at(1), 'e');
    FSL_CHECK_EQ(fostlib::string(u"Hello\x2014world!").at(4), 'o');
    FSL_CHECK_EQ(fostlib::string(u"Hello\x2014world!").at(5), 0x2014);
    FSL_CHECK_EQ(fostlib::string(u"Hello\x2014world!").at(6), 'w');

    FSL_CHECK_EQ(fostlib::string(u"\x5b6b").at(0), 0x5b6b);
    FSL_CHECK_EQ(fostlib::string(u"\xd834\xdd1e").at(0), 0x1d11e);
}


FSL_TEST_FUNCTION(lengths) {
    FSL_CHECK_EQ(fostlib::string(u"aaa").code_points(), 3u);
    FSL_CHECK_EQ(fostlib::string(u"a\x2014x").code_points(), 3u);
    FSL_CHECK_EQ(fostlib::string(u"Hello\x2014world!").code_points(), 12u);
    FSL_CHECK_EQ(fostlib::string(u"\xd834\xdd1e").code_points(), 1u);
    FSL_CHECK_EQ(fostlib::string(u"a\xd834\xdd1ex").code_points(), 3u);

    FSL_CHECK_EQ(fostlib::string(u"aaa").memory().size(), 3u);
    FSL_CHECK_EQ(fostlib::string(u"a\x2014x").memory().size(), 5u);
    FSL_CHECK_EQ(fostlib::string(u"Hello\x2014world!").memory().size(), 14u);
    FSL_CHECK_EQ(fostlib::string(u"\xd834\xdd1e").memory().size(), 4u);
    FSL_CHECK_EQ(fostlib::string(u"a\xd834\xdd1ex").memory().size(), 6u);
}


FSL_TEST_FUNCTION(casts) {
    fostlib::string const aaa{"aaa"};
    FSL_CHECK_EQ(static_cast<f5::u8view>(aaa), "aaa");
}


FSL_TEST_FUNCTION(substr) {
    FSL_CHECK_EQ(fostlib::string("abc").substr(), "abc");
    FSL_CHECK_EQ(fostlib::string("abc").substr(1, 0), "");
    FSL_CHECK_EQ(fostlib::string("abc").substr(1, 1), "b");
    FSL_CHECK_EQ(fostlib::string("abc").substr(0, 2), "ab");
    FSL_CHECK_EQ(fostlib::string("abc").substr(0, 10), "abc");
    FSL_CHECK_EQ(fostlib::string("abc").substr(4), "");
    FSL_CHECK_EQ(fostlib::string("abc").substr(fostlib::string::npos), "");

    FSL_CHECK_EQ(fostlib::string(u"a\x2014").substr(1, 1), u"\x2014");
    FSL_CHECK_EQ(fostlib::string(u"a\x2014x").substr(1, 1), u"\x2014");
    FSL_CHECK_EQ(
            fostlib::string(u"a\xd834\xdd1ex").substr(1, 1), u"\xd834\xdd1e");
}


FSL_TEST_FUNCTION(erase) {
    FSL_CHECK_EQ(fostlib::string("abc").erase(), "");
    FSL_CHECK_EQ(fostlib::string("abc").erase(1, 0), "abc");
    FSL_CHECK_EQ(fostlib::string("abc").erase(1, 1), "ac");
    FSL_CHECK_EQ(fostlib::string("abc").erase(0, 2), "c");
    FSL_CHECK_EQ(fostlib::string("abc").erase(0, 10), "");
    FSL_CHECK_EQ(fostlib::string("abc").erase(4), "");

    FSL_CHECK_EQ(fostlib::string(u"a\x2014").erase(1, 1), "a");
    FSL_CHECK_EQ(fostlib::string(u"a\x2014x").erase(1, 1), "ax");
    FSL_CHECK_EQ(fostlib::string(u"a\xd834\xdd1ex").erase(1, 1), "ax");
}


FSL_TEST_FUNCTION(insert) {
    FSL_CHECK_EQ(fostlib::string("abc").insert(0, "1"), "1abc");
    FSL_CHECK_EQ(fostlib::string("abc").insert(1, "2"), "a2bc");
    FSL_CHECK_EQ(fostlib::string("abc").insert(2, "3"), "ab3c");
    FSL_CHECK_EQ(fostlib::string("abc").insert(3, "4"), "abc4");
    FSL_CHECK_EQ(fostlib::string("abc").insert(4, "5"), "abc5");
}


FSL_TEST_FUNCTION(find) {
    FSL_CHECK_EQ(fostlib::string(u"Hello\x2014world!").find('w'), 6u);
    FSL_CHECK_EQ(fostlib::string(u"Hello\x2014world!").find('w', 4), 6u);
    FSL_CHECK_EQ(fostlib::string(u"Hello\x2014world!").find('w', 6), 6u);
    FSL_CHECK_EQ(fostlib::string(u"Hello\x2014world!").find("world"), 6u);
    FSL_CHECK_EQ(fostlib::string(u"Hello\x2014world!").find("world", 4), 6u);
    FSL_CHECK_EQ(fostlib::string(u"Hello\x2014world!").find("world", 6), 6u);
    FSL_CHECK_EQ(
            fostlib::string(u"Hello\x2014world!")
                    .find("world", fostlib::string::npos),
            fostlib::string::npos);
    FSL_CHECK_EQ(
            fostlib::string(u"Hello\x2014world!").find("wild"),
            fostlib::string::npos);
    FSL_CHECK_EQ(
            fostlib::string(u"Hello\x2014world!").find("wild", 4),
            fostlib::string::npos);
    FSL_CHECK_EQ(
            fostlib::string(u"Hello\x2014world!").find("wild", 6),
            fostlib::string::npos);
    FSL_CHECK_EQ(fostlib::string(u"Hello\x2014world!").find(0x2014), 5u);

    FSL_CHECK_EQ(
            fostlib::string(u"Hello\x2014world!").find_first_of(0x2014), 5u);
    FSL_CHECK_EQ(fostlib::string(u"Hello\x2014world!").find_first_of("xe"), 1u);
    FSL_CHECK_EQ(
            fostlib::string(u"Hello\x2014world!").find_first_not_of("He"), 2u);

    FSL_CHECK_EQ(
            fostlib::string(u"Hello\xd834\xdd1eworld!").find_first_of(0x1d11e),
            5u);
    FSL_CHECK_EQ(
            fostlib::string(u"Hello\xd834\xdd1eworld!").find_first_of("xe"),
            1u);
    FSL_CHECK_EQ(
            fostlib::string(u"Hello\xd834\xdd1eworld!").find_first_not_of("He"),
            2u);

    FSL_CHECK_EQ(fostlib::string("abc").find_last_not_of(" \n\t"), 2u);

    FSL_CHECK_EQ(fostlib::string("abc").find("."), fostlib::string::npos);
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
    FSL_CHECK(test.startswith("a"));
    FSL_CHECK(test.startswith("a "));
    FSL_CHECK(test.startswith("a test"));
    FSL_CHECK(!test.startswith("test"));
    FSL_CHECK(test.startswith(""));
    FSL_CHECK(!test.startswith("a test string which is long"));
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
            fostlib::string("abc").replace(1, 1, fostlib::string("xyz", 1, 1)),
            "ayc");
}


FSL_TEST_FUNCTION(utility_trim) {
    FSL_CHECK_NULL(fostlib::trim(f5::u8view{"   "}));

    FSL_CHECK_EQ(fostlib::trim(f5::u8view{"abc"}), "abc");
    FSL_CHECK_EQ(fostlib::trim(f5::u8view{"  abc"}), "abc");
    FSL_CHECK_EQ(fostlib::trim(f5::u8view{"  abc "}), "abc");

    FSL_CHECK_EQ(
            fostlib::trim(fostlib::string{u"Hello\x2014world!"}),
            u"Hello\x2014world!");
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
    FSL_CHECK_EQ(fostlib::split("abc", ".")[0], "abc");
    FSL_CHECK_EQ(fostlib::split("abc", "a")[0], "bc");
    FSL_CHECK_EQ(fostlib::split("abc", "b")[0], "a");
    FSL_CHECK_EQ(fostlib::split("abc", "b")[1], "c");
    FSL_CHECK_EQ(fostlib::split("abc", "c")[0], "ab");

    FSL_CHECK_EQ(fostlib::split("abc", "ab")[0], "c");
    FSL_CHECK_EQ(fostlib::split("abc", "bc")[0], "a");

    FSL_CHECK(fostlib::split("abc", "abc").empty());
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
