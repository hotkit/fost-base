/**
    Copyright 2008-2018, Felspar Co Ltd. <https://support.felspar.com/>

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

    FSL_CHECK_EQ(fostlib::json::parse(L"null"), fostlib::json());
    FSL_CHECK_EQ(fostlib::json::parse(L"false"), fostlib::json(false));
    FSL_CHECK_EQ(fostlib::json::parse(L"true"), fostlib::json(true));
    FSL_CHECK_EQ(fostlib::json::parse(L"10"), fostlib::json(10));
    FSL_CHECK_EQ(fostlib::json::parse(L"0"), fostlib::json(0));
    FSL_CHECK_EQ(fostlib::json::parse(L"-10"), fostlib::json(-10));
    FSL_CHECK_EQ(fostlib::json::parse(L"3.141"), fostlib::json(3.141));

    FSL_CHECK_EQ(fostlib::json::parse(L"\"Hello\""), fostlib::json(L"Hello"));
    FSL_CHECK_EQ(
            fostlib::json::parse(L"\"Hello\x2014world!\""),
            fostlib::json(L"Hello\x2014world!"));
    FSL_CHECK_EQ(
            fostlib::json::parse(L"\"He said, \\\"Hello world!\\\"\""),
            fostlib::json(L"He said, \"Hello world!\""));
    FSL_CHECK_EQ(
            fostlib::json::parse(L"\"Hello\\\\world\""),
            fostlib::json(L"Hello\\world"));
    FSL_CHECK_EQ(
            fostlib::json::parse(L"\"Hello\\/world\""),
            fostlib::json(L"Hello/world"));
    FSL_CHECK_EQ(
            fostlib::json::parse(L"\"Hello\\b\""),
            fostlib::json(L"Hello\x0008"));
    FSL_CHECK_EQ(
            fostlib::json::parse(L"\"Hello\\f\\b\""),
            fostlib::json(L"Hello\x000c\x0008"));
    FSL_CHECK_EQ(
            fostlib::json::parse(L"\"Hello\\n\""), fostlib::json(L"Hello\n"));
    FSL_CHECK_EQ(
            fostlib::json::parse(L"\"Hello\\r\""),
            fostlib::json(L"Hello\x000d"));
    FSL_CHECK_EQ(
            fostlib::json::parse(L"\"\tHello\\r\""),
            fostlib::json(L"\x0009Hello\x000d"));

    FSL_CHECK_EQ(fostlib::json::parse(emdash), fostlib::json(L"\x2014"));
    FSL_CHECK_EQ(
            fostlib::json::parse(L"\"\\u2014\""), fostlib::json(L"\x2014"));
    FSL_CHECK_EQ(fostlib::json::parse(suntzu), fostlib::json(L"\x5b6b\x5b50"));
    FSL_CHECK_EQ(
            fostlib::json::parse(L"\"\\u5b6b\\u5b50\""),
            fostlib::json(L"\x5b6b\x5b50"));
    FSL_CHECK_EQ(fostlib::json::parse(clef), fostlib::json(L"\xd834\xdd1e"));
    FSL_CHECK_EQ(
            fostlib::json::parse(L"\"\\ud834\\udd1e\""),
            fostlib::json(L"\xd834\xdd1e"));

    FSL_CHECK_EQ(
            fostlib::json::parse("\"\xd8\xa7\""), fostlib::json(L"\x0627"));
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
    FSL_CHECK_EQ(fostlib::json::parse(L"[]").size(), 0u);
    FSL_CHECK(fostlib::json::parse(L"[]").isarray());
    FSL_CHECK_EQ(fostlib::json::parse(L"[ ]").size(), 0u);
    FSL_CHECK_EQ(fostlib::json::parse(L"[\n]").size(), 0u);
    FSL_CHECK_EQ(fostlib::json::parse(L"[\n ]").size(), 0u);
    FSL_CHECK_EQ(fostlib::json::parse(L"[\t]").size(), 0u);
    FSL_CHECK_EQ(fostlib::json::parse(L"[ null ]").size(), 1u);
    FSL_CHECK_EQ(fostlib::json::parse(L"[ null, null ]").size(), 2u);
    FSL_CHECK_EQ(fostlib::json::parse(L"[ null, null, null ]").size(), 3u);
    FSL_CHECK_EQ(fostlib::json::parse(L"[null ]")[0], fostlib::json());
    FSL_CHECK_EQ(fostlib::json::parse(L"[-10]").size(), 1u);
    FSL_CHECK_EQ(fostlib::json::parse(L"[-10]")[0], fostlib::json(-10));
    FSL_CHECK_EQ(fostlib::json::parse(L"[true]")[0], fostlib::json(true));
}


FSL_TEST_FUNCTION(json_array_nested) {
    FSL_CHECK_EQ(fostlib::json::parse(L"[[]]").size(), 1u);
    FSL_CHECK_EQ(fostlib::json::parse(L"[[]]")[0].size(), 0u);

    FSL_CHECK_EQ(fostlib::json::parse(L"[[10]]")[0][0], fostlib::json(10));
    FSL_CHECK_EQ(fostlib::json::parse(L"[1234, [10]]")[0], fostlib::json(1234));
    FSL_CHECK_EQ(
            fostlib::json::parse(L"[1234, [10]]")[1][0], fostlib::json(10));
    FSL_CHECK_EQ(
            fostlib::json::parse(L"[1234, [10]]")[1],
            fostlib::json::parse(L"[10 ]"));

    FSL_CHECK_EQ(
            fostlib::json::parse(L"[[-10,1234]]")[0][0], fostlib::json(-10));
    FSL_CHECK_EQ(
            fostlib::json::parse(L"[[-10,1234]]")[0][1], fostlib::json(1234));

    FSL_CHECK_EQ(
            fostlib::json::parse(L"[[-10,1234], [10]]")[0][0],
            fostlib::json(-10));
    FSL_CHECK_EQ(
            fostlib::json::parse(L"[[-10,1234], [10]]")[0][1],
            fostlib::json(1234));
    FSL_CHECK_EQ(
            fostlib::json::parse(L"[[-10,1234], [10]]")[1][0],
            fostlib::json(10));

    FSL_CHECK_EQ(
            fostlib::json::parse(L"[[-10,1234],[10],[44,55,66,77]]")[0][0],
            fostlib::json(-10));
    FSL_CHECK_EQ(
            fostlib::json::parse(L"[[-10,1234],[10],[44,55,66,77]]")[0][1],
            fostlib::json(1234));
    FSL_CHECK_EQ(
            fostlib::json::parse(L"[[-10,1234],[10],[44,55,66,77]]")[1][0],
            fostlib::json(10));
    FSL_CHECK_EQ(
            fostlib::json::parse(L"[[-10,1234],[10],[44,55,66,77]]")[2].size(),
            4u);

    fostlib::json pi(
            fostlib::json::parse(L"[[3,33,333],[1],[4,44,444,4444],[11]]"));
    FSL_CHECK_NEQ(pi, fostlib::json(true));
    FSL_CHECK_EQ(pi[0].size(), 3u);
    FSL_CHECK_EQ(pi[1].size(), 1u);
    FSL_CHECK_EQ(pi[2].size(), 4u);
    FSL_CHECK_EQ(pi[3].size(), 1u);
}


FSL_TEST_FUNCTION(json_object) {
    FSL_CHECK_EQ(
            fostlib::json::parse(L"{}"),
            fostlib::json(fostlib::json::object_t()));
    FSL_CHECK(fostlib::json::parse(L"{}").isobject());
    FSL_CHECK(fostlib::json::parse(L"{ }").isobject());
    FSL_CHECK(fostlib::json::parse(L"{\n}").isobject());
    FSL_CHECK(fostlib::json::parse(L"{\n }").isobject());
    FSL_CHECK(fostlib::json::parse(L"{\t}").isobject());

    FSL_CHECK(fostlib::json::parse(L"{\"key\":\"value\"}").isobject());
    FSL_CHECK_EQ(
            fostlib::json::parse(L"{\"key\":\"value\"}")[L"key"],
            fostlib::json(L"value"));
    FSL_CHECK(fostlib::json::parse(L"{\"0\":\"value 1\", \"1\":\"value 2\"}")
                      .isobject());
    FSL_CHECK_EQ(
            fostlib::json::parse(
                    L"{\"0\":\"value 1\", \"1\":\"value 2\"}")[L"0"],
            fostlib::json(L"value 1"));
    FSL_CHECK_EQ(
            fostlib::json::parse(
                    L"{\"0\":\"value 1\", \"1\":\"value 2\"}")[L"1"],
            fostlib::json(L"value 2"));
}


FSL_TEST_FUNCTION(tortuous) {
    fostlib::json p(
            fostlib::json::parse(L"\
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
    FSL_CHECK_EQ(p[3][L"id"], fostlib::json(1234));
    FSL_CHECK_EQ(p[3][L"type"], fostlib::json(L"FSLib::Type"));

    FSL_CHECK(p[4].isobject());
    FSL_CHECK_EQ(p[4][L"latitude"], fostlib::json(6.234));
    FSL_CHECK_EQ(p[4][L"longitude"], fostlib::json(53.12353));

    FSL_CHECK_EQ(
            fostlib::json::unparse(p, false),
            L"[10,20,[true,false],{\"id\":1234,\"type\":\"FSLib::Type\"},{"
            L"\"latitude\":6.234,\"longitude\":53.123530000000002}]");
}


FSL_TEST_FUNCTION(unparse) {
    FSL_CHECK_EQ(L"null", fostlib::json::unparse(fostlib::json(), false));
    FSL_CHECK_EQ(L"true", fostlib::json::unparse(fostlib::json(true), false));
    FSL_CHECK_EQ(L"false", fostlib::json::unparse(fostlib::json(false), false));
    FSL_CHECK_EQ(L"10123", fostlib::json::unparse(fostlib::json(10123), false));
    FSL_CHECK_EQ(L"1.5", fostlib::json::unparse(fostlib::json(1.5), false));
    FSL_CHECK_EQ(
            L"\"1.5\"", fostlib::json::unparse(fostlib::json(L"1.5"), false));
    FSL_CHECK_EQ(
            L"\"\\n\\n\"",
            fostlib::json::unparse(fostlib::json(L"\n\n"), false));
    FSL_CHECK_EQ(
            L"\"\\r\\n\"",
            fostlib::json::unparse(fostlib::json(L"\r\n"), false));
    FSL_CHECK_EQ(
            "\"\\u000b\"",
            fostlib::json::unparse(fostlib::json(L"\x000b"), false));
    FSL_CHECK_EQ(
            "\"\xE2\x80\x94\"",
            fostlib::json::unparse(fostlib::json("\xE2\x80\x94"), false));
    FSL_CHECK_EQ(
            "\"\xD8\xA7\"",
            fostlib::json::unparse(fostlib::json(L"\x0627"), false));
    FSL_CHECK_EQ(
            "\"\xF0\x9D\x84\x9E\"",
            fostlib::json::unparse(fostlib::json(L"\xd834\xdd1e"), false));
}

FSL_TEST_FUNCTION(sloppy_json) {



        FSL_CHECK_EQ(fostlib::json::parse("//test\n15"), fostlib::json(15));
        FSL_CHECK_EQ(fostlib::json::parse("//test\n//test\n15"), fostlib::json(15));
        FSL_CHECK_EQ(fostlib::json::parse("15//test\n"), fostlib::json(15));
        FSL_CHECK_EQ(fostlib::json::parse("//test\n 15"), fostlib::json(15));
        FSL_CHECK_EQ(fostlib::json::parse("//test\n15.6"), fostlib::json(15.6));
        FSL_CHECK_EQ(fostlib::json::parse("//test\n//test\n15.6"), fostlib::json(15.6));
        FSL_CHECK_EQ(fostlib::json::parse("15.6//test\n"), fostlib::json(15.6));

        FSL_CHECK_EQ(fostlib::json::parse("15.6 \n\t//test\n"), fostlib::json(15.6));

        FSL_CHECK_EQ(fostlib::json::parse("//test\nfalse"), fostlib::json(false));
        FSL_CHECK_EQ(fostlib::json::parse("//test\n//test\nfalse"), fostlib::json(false));
        FSL_CHECK_EQ(fostlib::json::parse("false \n\t//test\n"), fostlib::json(false));
        FSL_CHECK_EQ(fostlib::json::parse("//test\ntrue"), fostlib::json(true));
        FSL_CHECK_EQ(fostlib::json::parse("//test\n//test\ntrue"), fostlib::json(true));
        FSL_CHECK_EQ(fostlib::json::parse("true \n\t//test\n"), fostlib::json(true));

        FSL_CHECK_EQ(fostlib::json::parse("//test\nnull"), fostlib::json());
        FSL_CHECK_EQ(fostlib::json::parse("//test\n//test\nnull"), fostlib::json());
        FSL_CHECK_EQ(fostlib::json::parse("null//test\n//test\n"), fostlib::json());


        FSL_CHECK_EQ(fostlib::json::parse("//test\n[1, \"2\", true]")[0], fostlib::json(1));
        FSL_CHECK_EQ(fostlib::json::parse("[1, \"2\", true]//test\n")[2], fostlib::json(true));

        FSL_CHECK_EQ(fostlib::json::parse("{\"key\":\"value\"}//test\n")["key"], fostlib::json("value"));
        FSL_CHECK_EQ(fostlib::json::parse("//test\n{\"key\":\"value\"}")["key"], fostlib::json("value"));

        FSL_CHECK_EQ(fostlib::json::parse("//test\n\"string\""), fostlib::json("string"));
        FSL_CHECK_EQ(fostlib::json::parse("\"string\"//test\n"), fostlib::json("string"));

        FSL_CHECK_EQ(fostlib::json::parse("//test\n{\"key\"://test\"fake-value\"\n\"real-value\"}")["key"], fostlib::json("real-value"));
        FSL_CHECK_EQ(fostlib::json::parse("//test\n{\"key\":  \"value //\"\n}")["key"], fostlib::json("value //"));
        FSL_CHECK_EQ(fostlib::json::parse("[//test\n1, \"2\", true]")[0], fostlib::json(1));
        FSL_CHECK_EQ(fostlib::json::parse("[1, \"2\", true//test\n]")[2], fostlib::json(true));

        FSL_CHECK_EQ(fostlib::json::parse("/*test*/null"), fostlib::json());
        FSL_CHECK_EQ(fostlib::json::parse("5/*test*/"), fostlib::json(5));
        // FSL_CHECK_EQ(fostlib::json::parse("5/**/6"), fostlib::json(56));
        // FSL_CHECK_EQ(fostlib::json::parse("/**/\"a\"/**/\"b\"/**/"), fostlib::json("ab"));
}
