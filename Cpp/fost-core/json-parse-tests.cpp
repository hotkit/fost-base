/**
    Copyright 2008-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <fost/array>
#include "fost-core-test.hpp"


#include <fost/exception/parse_error.hpp>
#include <fost/exception/unicode_encoding.hpp>


FSL_TEST_SUITE(json_parse);


FSL_TEST_FUNCTION(atoms) {
    std::array<unsigned char, 5> emdash{{'"', 0xE2, 0x80, 0x94, '"'}};
    std::array<unsigned char, 8> suntzu{
            {'"', 0xE5, 0xAD, 0xAB, 0xE5, 0xAD, 0x90, '"'}};
    std::array<unsigned char, 6> clef{{'"', 0xF0, 0x9D, 0x84, 0x9E, '"'}};

    FSL_CHECK_EQ(fostlib::json::parse("null"), fostlib::json());
    FSL_CHECK_EQ(fostlib::json::parse("false"), fostlib::json(false));
    FSL_CHECK_EQ(fostlib::json::parse("true"), fostlib::json(true));
    FSL_CHECK_EQ(fostlib::json::parse("10"), fostlib::json(10));
    FSL_CHECK_EQ(fostlib::json::parse("0"), fostlib::json(0));
    FSL_CHECK_EQ(fostlib::json::parse("-10"), fostlib::json(-10));
    FSL_CHECK_EQ(fostlib::json::parse("3.141"), fostlib::json(3.141));

    FSL_CHECK_EQ(fostlib::json::parse("\"Hello\""), fostlib::json("Hello"));
    FSL_CHECK_EQ(
            fostlib::json::parse(u"\"Hello\x2014world!\""),
            fostlib::json(u"Hello\x2014world!"));
    FSL_CHECK_EQ(
            fostlib::json::parse("\"He said, \\\"Hello world!\\\"\""),
            fostlib::json("He said, \"Hello world!\""));
    FSL_CHECK_EQ(
            fostlib::json::parse("\"Hello\\\\world\""),
            fostlib::json("Hello\\world"));
    FSL_CHECK_EQ(
            fostlib::json::parse("\"Hello\\/world\""),
            fostlib::json("Hello/world"));
    FSL_CHECK_EQ(
            fostlib::json::parse("\"Hello\\b\""),
            fostlib::json(u"Hello\x0008"));
    FSL_CHECK_EQ(
            fostlib::json::parse("\"Hello\\f\\b\""),
            fostlib::json(u"Hello\x000c\x0008"));
    FSL_CHECK_EQ(
            fostlib::json::parse("\"Hello\\n\""), fostlib::json("Hello\n"));
    FSL_CHECK_EQ(
            fostlib::json::parse("\"Hello\\r\""),
            fostlib::json(u"Hello\x000d"));
    FSL_CHECK_EQ(
            fostlib::json::parse("\"\tHello\\r\""),
            fostlib::json(u"\x0009Hello\x000d"));

    FSL_CHECK_EQ(fostlib::json::parse(emdash), fostlib::json(u"\x2014"));
    FSL_CHECK_EQ(
            fostlib::json::parse("\"\\u2014\""), fostlib::json(u"\x2014"));
    FSL_CHECK_EQ(fostlib::json::parse(suntzu), fostlib::json(u"\x5b6b\x5b50"));
    FSL_CHECK_EQ(
            fostlib::json::parse("\"\\u5b6b\\u5b50\""),
            fostlib::json(u"\x5b6b\x5b50"));
    FSL_CHECK_EQ(fostlib::json::parse(clef), fostlib::json(u"\xd834\xdd1e"));
    FSL_CHECK_EQ(
            fostlib::json::parse("\"\\ud834\\udd1e\""),
            fostlib::json(u"\xd834\xdd1e"));

    FSL_CHECK_EQ(
            fostlib::json::parse("\"\xd8\xa7\""), fostlib::json(u"\x0627"));
}


FSL_TEST_FUNCTION(json_broken) {
    using parse_error = fostlib::exceptions::parse_error;
    using unicode_error = fostlib::exceptions::unicode_encoding;

    FSL_CHECK_EXCEPTION(fostlib::json::parse(""), parse_error &);

    FSL_CHECK_EXCEPTION(fostlib::json::parse("\"Hello\" world"), parse_error &);
    FSL_CHECK_EXCEPTION(fostlib::json::parse("\"He\\llo\""), parse_error &);

    FSL_CHECK_EXCEPTION(fostlib::json::parse("\"\\ud834\""), unicode_error &);

    /// Extra test cases taken from "Parsing JSON is a Minefield"
    /// http://seriot.ch/parsing_json.html
    FSL_CHECK_EXCEPTION(fostlib::json::parse("{\"id\":0,}"), parse_error &);
    FSL_CHECK_EXCEPTION(
            fostlib::json::parse("{\"id\":0,,,,,,,}"), parse_error &);

    FSL_CHECK_EXCEPTION(
            fostlib::json::parse("{\"a\":\"b\"}/**/"), parse_error &);
    FSL_CHECK_EXCEPTION(
            fostlib::json::parse("{\"a\":/*comment*/\"b\"}"), parse_error &);

    FSL_CHECK_EXCEPTION(fostlib::json::parse("{\"\":"), parse_error &);
    FSL_CHECK_EXCEPTION(fostlib::json::parse("{}}"), parse_error &);
    //     FSL_CHECK_EXCEPTION(
    //             fostlib::json::parse(f5::u8view{std::string(10000, '[')}),
    //             parse_error &);

    FSL_CHECK_EXCEPTION(fostlib::json::parse("[0x42]"), parse_error &);

    FSL_CHECK_EXCEPTION(fostlib::json::parse("[\"\\"), parse_error &);

    FSL_CHECK_EXCEPTION(fostlib::json::parse("[\"a\\\x09\"]"), parse_error &);
}


FSL_TEST_FUNCTION(json_array) {
    FSL_CHECK_EQ(fostlib::json::parse("[]").size(), 0u);
    FSL_CHECK(fostlib::json::parse("[]").isarray());
    FSL_CHECK_EQ(fostlib::json::parse("[ ]").size(), 0u);
    FSL_CHECK_EQ(fostlib::json::parse("[\n]").size(), 0u);
    FSL_CHECK_EQ(fostlib::json::parse("[\n ]").size(), 0u);
    FSL_CHECK_EQ(fostlib::json::parse("[\t]").size(), 0u);
    FSL_CHECK_EQ(fostlib::json::parse("[ null ]").size(), 1u);
    FSL_CHECK_EQ(fostlib::json::parse("[ null, null ]").size(), 2u);
    FSL_CHECK_EQ(fostlib::json::parse("[ null, null, null ]").size(), 3u);
    FSL_CHECK_EQ(fostlib::json::parse("[null ]")[0], fostlib::json());
    FSL_CHECK_EQ(fostlib::json::parse("[-10]").size(), 1u);
    FSL_CHECK_EQ(fostlib::json::parse("[-10]")[0], fostlib::json(-10));
    FSL_CHECK_EQ(fostlib::json::parse("[true]")[0], fostlib::json(true));
}


FSL_TEST_FUNCTION(json_array_nested) {
    FSL_CHECK_EQ(fostlib::json::parse("[[]]").size(), 1u);
    FSL_CHECK_EQ(fostlib::json::parse("[[]]")[0].size(), 0u);

    FSL_CHECK_EQ(fostlib::json::parse("[[10]]")[0][0], fostlib::json(10));
    FSL_CHECK_EQ(fostlib::json::parse("[1234, [10]]")[0], fostlib::json(1234));
    FSL_CHECK_EQ(
            fostlib::json::parse("[1234, [10]]")[1][0], fostlib::json(10));
    FSL_CHECK_EQ(
            fostlib::json::parse("[1234, [10]]")[1],
            fostlib::json::parse("[10 ]"));

    FSL_CHECK_EQ(
            fostlib::json::parse("[[-10,1234]]")[0][0], fostlib::json(-10));
    FSL_CHECK_EQ(
            fostlib::json::parse("[[-10,1234]]")[0][1], fostlib::json(1234));

    FSL_CHECK_EQ(
            fostlib::json::parse("[[-10,1234], [10]]")[0][0],
            fostlib::json(-10));
    FSL_CHECK_EQ(
            fostlib::json::parse("[[-10,1234], [10]]")[0][1],
            fostlib::json(1234));
    FSL_CHECK_EQ(
            fostlib::json::parse("[[-10,1234], [10]]")[1][0],
            fostlib::json(10));

    FSL_CHECK_EQ(
            fostlib::json::parse("[[-10,1234],[10],[44,55,66,77]]")[0][0],
            fostlib::json(-10));
    FSL_CHECK_EQ(
            fostlib::json::parse("[[-10,1234],[10],[44,55,66,77]]")[0][1],
            fostlib::json(1234));
    FSL_CHECK_EQ(
            fostlib::json::parse("[[-10,1234],[10],[44,55,66,77]]")[1][0],
            fostlib::json(10));
    FSL_CHECK_EQ(
            fostlib::json::parse("[[-10,1234],[10],[44,55,66,77]]")[2].size(),
            4u);

    fostlib::json pi(
            fostlib::json::parse("[[3,33,333],[1],[4,44,444,4444],[11]]"));
    FSL_CHECK_NEQ(pi, fostlib::json(true));
    FSL_CHECK_EQ(pi[0].size(), 3u);
    FSL_CHECK_EQ(pi[1].size(), 1u);
    FSL_CHECK_EQ(pi[2].size(), 4u);
    FSL_CHECK_EQ(pi[3].size(), 1u);
}


FSL_TEST_FUNCTION(json_object) {
    FSL_CHECK_EQ(
            fostlib::json::parse("{}"),
            fostlib::json(fostlib::json::object_t()));
    FSL_CHECK(fostlib::json::parse("{}").isobject());
    FSL_CHECK(fostlib::json::parse("{ }").isobject());
    FSL_CHECK(fostlib::json::parse("{\n}").isobject());
    FSL_CHECK(fostlib::json::parse("{\n }").isobject());
    FSL_CHECK(fostlib::json::parse("{\t}").isobject());

    FSL_CHECK(fostlib::json::parse("{\"key\":\"value\"}").isobject());
    FSL_CHECK_EQ(
            fostlib::json::parse("{\"key\":\"value\"}")["key"],
            fostlib::json("value"));
    FSL_CHECK(fostlib::json::parse("{\"0\":\"value 1\", \"1\":\"value 2\"}")
                      .isobject());
    FSL_CHECK_EQ(
            fostlib::json::parse(
                    "{\"0\":\"value 1\", \"1\":\"value 2\"}")["0"],
            fostlib::json("value 1"));
    FSL_CHECK_EQ(
            fostlib::json::parse(
                    "{\"0\":\"value 1\", \"1\":\"value 2\"}")["1"],
            fostlib::json("value 2"));
}


FSL_TEST_FUNCTION(tortuous) {
    fostlib::json p(
            fostlib::json::parse("\
[\
    10,\
    20,\
    [ true, false ],\
    {\
        \"id\": 1234,\
        \"type\": \"FSLib::Type\"\
    },\
    { \"latitude\": 6.234, \"longitude\": 53.12353 }\
]\
            "));

    FSL_CHECK(p.isarray());
    FSL_CHECK_EQ(p.size(), 5u);

    FSL_CHECK(p.isarray());
    FSL_CHECK(p[0].isatom());
    FSL_CHECK(p[1].isatom());
    FSL_CHECK(p[2].isarray());
    FSL_CHECK(p[3].isobject());
    FSL_CHECK(p[4].isobject());

    FSL_CHECK_EQ(p[0], fostlib::json(10));
    FSL_CHECK_EQ(p[1], fostlib::json(20));

    FSL_CHECK(p[2].isarray());
    FSL_CHECK_EQ(p[2][0], fostlib::json(true));
    FSL_CHECK_EQ(p[2][1], fostlib::json(false));

    FSL_CHECK(p[3].isobject());
    FSL_CHECK_EQ(p[3]["id"], fostlib::json(1234));
    FSL_CHECK_EQ(p[3]["type"], fostlib::json("FSLib::Type"));

    FSL_CHECK(p[4].isobject());
    FSL_CHECK_EQ(p[4]["latitude"], fostlib::json(6.234));
    FSL_CHECK_EQ(p[4]["longitude"], fostlib::json(53.12353));

    FSL_CHECK_EQ(
            fostlib::json::unparse(p, false),
            "[10,20,[true,false],{\"id\":1234,\"type\":\"FSLib::Type\"},{"
            "\"latitude\":6.234,\"longitude\":53.123530000000002}]");
}


FSL_TEST_FUNCTION(unparse) {
    FSL_CHECK_EQ("null", fostlib::json::unparse(fostlib::json(), false));
    FSL_CHECK_EQ("true", fostlib::json::unparse(fostlib::json(true), false));
    FSL_CHECK_EQ("false", fostlib::json::unparse(fostlib::json(false), false));
    FSL_CHECK_EQ("10123", fostlib::json::unparse(fostlib::json(10123), false));
    FSL_CHECK_EQ("1.5", fostlib::json::unparse(fostlib::json(1.5), false));
    FSL_CHECK_EQ(
            "\"1.5\"", fostlib::json::unparse(fostlib::json("1.5"), false));
    FSL_CHECK_EQ(
            "\"\\n\\n\"",
            fostlib::json::unparse(fostlib::json("\n\n"), false));
    FSL_CHECK_EQ(
            "\"\\r\\n\"",
            fostlib::json::unparse(fostlib::json("\r\n"), false));
    FSL_CHECK_EQ(
            "\"\\u000b\"",
            fostlib::json::unparse(fostlib::json(u"\x000b"), false));
    FSL_CHECK_EQ(
            "\"\xE2\x80\x94\"",
            fostlib::json::unparse(fostlib::json("\xE2\x80\x94"), false));
    FSL_CHECK_EQ(
            "\"\xD8\xA7\"",
            fostlib::json::unparse(fostlib::json(u"\x0627"), false));
    FSL_CHECK_EQ(
            "\"\xF0\x9D\x84\x9E\"",
            fostlib::json::unparse(fostlib::json(u"\xd834\xdd1e"), false));
}
