/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-core-test.hpp"

#include <fost/exception/parse_error.hpp>

FSL_TEST_SUITE(json_sloppy_parse);

FSL_TEST_FUNCTION(atoms) {

    FSL_CHECK_EQ(fostlib::json::sloppy_parse("//test\n15"), fostlib::json(15));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("//test\n//test\n15"),
            fostlib::json(15));
    FSL_CHECK_EQ(fostlib::json::sloppy_parse("15//test\n"), fostlib::json(15));
    FSL_CHECK_EQ(fostlib::json::sloppy_parse("//test\n 15"), fostlib::json(15));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("//test\n15.6"), fostlib::json(15.6));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("//test\n//test\n15.6"),
            fostlib::json(15.6));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("15.6//test\n"), fostlib::json(15.6));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("15.6 \n\t//test\n"),
            fostlib::json(15.6));

    FSL_CHECK_EQ(fostlib::json::sloppy_parse("5/*test*/"), fostlib::json(5));
    FSL_CHECK_EQ(fostlib::json::sloppy_parse("/*test*/5"), fostlib::json(5));

    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("//test\nfalse"), fostlib::json(false));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("//test\n//test\nfalse"),
            fostlib::json(false));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("false \n\t//test\n"),
            fostlib::json(false));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("//test\ntrue"), fostlib::json(true));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("//test\n//test\ntrue"),
            fostlib::json(true));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("true \n\t//test\n"),
            fostlib::json(true));

    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("false/*test*/"), fostlib::json(false));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("/*test*/true"), fostlib::json(true));

    FSL_CHECK_EQ(fostlib::json::sloppy_parse("//test\nnull"), fostlib::json());
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("//test\n//test\nnull"),
            fostlib::json());
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("null//test\n//test\n"),
            fostlib::json());

    FSL_CHECK_EQ(fostlib::json::sloppy_parse("/*test*/null"), fostlib::json());
    FSL_CHECK_EQ(fostlib::json::sloppy_parse("null/*test*/"), fostlib::json());

    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("//test\n\"string\""),
            fostlib::json("string"));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("\"string\"//test\n"),
            fostlib::json("string"));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("\"string\"//test"),
            fostlib::json("string"));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("\"string\"///**/"),
            fostlib::json("string"));

    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("\"string\"/*test*/"),
            fostlib::json("string"));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("/*test*/\"string\""),
            fostlib::json("string"));
}

FSL_TEST_FUNCTION(json_array) {
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("//test\n[1, \"2\", true]")[0],
            fostlib::json(1));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("[1, \"2\", true]//test\n")[2],
            fostlib::json(true));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("[//test\n1, \"2\", true]")[0],
            fostlib::json(1));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("[1, \"2\", true//test\n]")[2],
            fostlib::json(true));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("[1, //test\n\"2\", true]")[2],
            fostlib::json(true));

    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("/*test*/\n[1, \"2\", true]")[0],
            fostlib::json(1));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("[1, \"2\", true]/*test*/")[2],
            fostlib::json(true));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("[/*test*/1, \"2\", true]")[0],
            fostlib::json(1));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("[1, \"2\", true/*test*/]")[2],
            fostlib::json(true));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("[1, /*test*/\"2\", true]")[2],
            fostlib::json(true));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("[1, /*//test*/\n\"2\", true]")[2],
            fostlib::json(true));

    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("[1, /*//test*/\n\"2\", true]")[2],
            fostlib::json(true));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("[1, //3\n\"2\", true]")[2],
            fostlib::json(true));

    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("[1, 2, 3,]")[2], fostlib::json(3));
    FSL_CHECK_EQ(fostlib::json::sloppy_parse("[1 2 3]")[2], fostlib::json(3));
    FSL_CHECK_EQ(fostlib::json::sloppy_parse("[1\n2\n3]")[2], fostlib::json(3));
    FSL_CHECK_EQ(fostlib::json::sloppy_parse("[1\t2\t3]")[2], fostlib::json(3));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("[1/*test*/\n3]")[1], fostlib::json(3));
}

FSL_TEST_FUNCTION(json_array_nested) {
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("[[0,1] //test\n]")[0][0],
            fostlib::json(0));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("[[0, //test\n 1]]")[0][0],
            fostlib::json(0));

    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("[[0, /*test*/ 1]]")[0][0],
            fostlib::json(0));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("[[0,1] /*test*/]")[0][0],
            fostlib::json(0));

    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("[[0,1,],]")[0][0], fostlib::json(0));
}

FSL_TEST_FUNCTION(json_object) {
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("{\"key\":\"value\"}//test\n")["key"],
            fostlib::json("value"));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("//test\n{\"key\":\"value\"}")["key"],
            fostlib::json("value"));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse(
                    "//test\n{\"key\"://\"fake-value\"\n\"real-value\"}")
                    ["key"],
            fostlib::json("real-value"));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse(
                    "//test\n{\"key\":  \"value //\"\n}")["key"],
            fostlib::json("value //"));

    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("{\"key\":\"value\", /*fake-value*/ "
                                        "\"key2\":\"real-value\"}")["key2"],
            fostlib::json("real-value"));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("{\"key\":\"value\"}/*test*/")["key"],
            fostlib::json("value"));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse("/*test*/{\"key\":\"value\"}")["key"],
            fostlib::json("value"));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse(
                    "/*test*/{\"key\":  \"value /**/\"}")["key"],
            fostlib::json("value /**/"));

    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse(
                    "{\"key\":\"value\",\"key2\":\"value2\",}")["key"],
            fostlib::json("value"));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse(
                    "{\"key\":\"value\" \"key2\":\"value2\"}")["key"],
            fostlib::json("value"));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse(
                    "{\"key\":\"value\"\n\"key2\":\"value2\"}")["key2"],
            fostlib::json("value2"));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse(
                    "{\"key\":\"value\"\t\"key2\":\"value2\"}")["key2"],
            fostlib::json("value2"));
    FSL_CHECK_EQ(
            fostlib::json::sloppy_parse(
                    "{\"key\":\"value\"/*test*/\n\"key2\":\"value2\"}")["key2"],
            fostlib::json("value2"));
}

FSL_TEST_FUNCTION(sloppy_json_broken) {
    using parse_error = fostlib::exceptions::parse_error;
    FSL_CHECK_EXCEPTION(fostlib::json::sloppy_parse("5/**/6"), parse_error &);
    FSL_CHECK_EXCEPTION(
            fostlib::json::sloppy_parse("\"a\"/**/\"b\""), parse_error &);
    FSL_CHECK_EXCEPTION(
            fostlib::json::sloppy_parse("/* test   "), parse_error &);
    FSL_CHECK_EXCEPTION(
            fostlib::json::sloppy_parse("/*/* */*/"), parse_error &);
    FSL_CHECK_EXCEPTION(
            fostlib::json::sloppy_parse("[ test// value]"), parse_error &);
}
