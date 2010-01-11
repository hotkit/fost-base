/*
    Copyright 2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"


FSL_TEST_SUITE( coercion_int );


namespace {
    class static_asserts {
        typedef fostlib::detail::coerce_int_equal_size_same_sign<
            char, char
        >::type char_char;
        typedef fostlib::detail::coerce_int_equal_size_same_sign<
            int, int
        >::type int_int;
        typedef fostlib::detail::coerce_int_equal_size_same_sign<
            uint64_t, uint64_t
        >::type uint64_t_uint64_t;

        typedef fostlib::detail::coerce_int_T_larger_signed_F_smaller_unsigned<
            int64_t, unsigned char
        >:: type int64_t_unsigned_char;
    };
}


FSL_TEST_FUNCTION( from_same_to_same ) {
    signed char c1 = 0, c2 = -10, c3 = 10;
    unsigned char uc1 = 0, uc2 = 10;
    int64_t i1 = 0, i2 = -10, i3 = 10, i4 = -1000, i5 = 1000;
    uint64_t u1 = 0, u2 = 10, u3 = 1000;

    FSL_CHECK_EQ( fostlib::coerce< signed char >( c1 ), c1 );
    FSL_CHECK_EQ( fostlib::coerce< signed char >( c2 ), c2 );
    FSL_CHECK_EQ( fostlib::coerce< signed char >( c3 ), c3 );

    FSL_CHECK_EQ( fostlib::coerce< unsigned char >( uc1 ), uc1 );
    FSL_CHECK_EQ( fostlib::coerce< unsigned char >( uc2 ), uc2 );

    FSL_CHECK_EQ( fostlib::coerce< int64_t >( i1 ), i1 );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( i2 ), i2 );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( i3 ), i3 );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( i4 ), i4 );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( i5 ), i5 );

    FSL_CHECK_EQ( fostlib::coerce< uint64_t >( u1 ), u1 );
    FSL_CHECK_EQ( fostlib::coerce< uint64_t >( u2 ), u2 );
    FSL_CHECK_EQ( fostlib::coerce< uint64_t >( u3 ), u3 );
}


FSL_TEST_FUNCTION( from_small_to_larger ) {
    signed char c1 = 0, c2 = -10, c3 = 10;
    unsigned char uc1 = 0, uc2 = 10;
    int64_t i1 = 0, i2 = -10, i3 = 10, i4 = -1000, i5 = 1000;
    uint64_t u1 = 0, u2 = 10, u3 = 1000;

    FSL_CHECK_EQ( fostlib::coerce< int64_t >( c1 ), i1 );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( c2 ), i2 );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( c3 ), i3 );

    FSL_CHECK_EQ( fostlib::coerce< int64_t >( uc1 ), i1 );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( uc2 ), i3 );

    FSL_CHECK_EQ( fostlib::coerce< uint64_t >( i1 ), u1 );
    FSL_CHECK_EXCEPTION(
        fostlib::coerce< uint64_t >( i2 ),
        fostlib::exceptions::out_of_range_string&
    );
    FSL_CHECK_EQ( fostlib::coerce< uint64_t >( i3 ), u2 );
    FSL_CHECK_EXCEPTION(
        fostlib::coerce< uint64_t >( i4 ),
        fostlib::exceptions::out_of_range_string&
    );
    FSL_CHECK_EQ( fostlib::coerce< uint64_t >( i5 ), u3 );
}


FSL_TEST_FUNCTION( from_large_to_smaller ) {
    signed char c1 = 0, c2 = -10, c3 = 10;
    unsigned char uc1 = 0, uc2 = 10;
    int64_t i1 = 0, i2 = -10, i3 = 10, i4 = -1000, i5 = 1000;
    uint64_t u1 = 0, u2 = 10, u3 = 1000;

    FSL_CHECK_EQ( fostlib::coerce< signed char >( i1 ), c1 );
    FSL_CHECK_EQ( fostlib::coerce< signed char >( i2 ), c2 );
    FSL_CHECK_EQ( fostlib::coerce< signed char >( i3 ), c3 );
    FSL_CHECK_EXCEPTION(
        fostlib::coerce< signed char >( i4 ),
        fostlib::exceptions::out_of_range_string&
    );
    FSL_CHECK_EXCEPTION(
        fostlib::coerce< signed char >( i5 ),
        fostlib::exceptions::out_of_range_string&
    );

    FSL_CHECK_EQ( fostlib::coerce< signed char >( u1 ), c1 );
    FSL_CHECK_EQ( fostlib::coerce< signed char >( u2 ), c3 );
    FSL_CHECK_EXCEPTION(
        fostlib::coerce< signed char >( u3 ),
        fostlib::exceptions::out_of_range_string&
    );

    FSL_CHECK_EQ( fostlib::coerce< unsigned char >( i1 ), uc1 );
    FSL_CHECK_EXCEPTION(
        fostlib::coerce< unsigned char >( i2 ),
        fostlib::exceptions::out_of_range_string&
    );
    FSL_CHECK_EQ( fostlib::coerce< unsigned char >( i3 ), uc2 );
    FSL_CHECK_EXCEPTION(
        fostlib::coerce< unsigned char >( i4 ),
        fostlib::exceptions::out_of_range_string&
    );
    FSL_CHECK_EXCEPTION(
        fostlib::coerce< unsigned char >( i5 ),
        fostlib::exceptions::out_of_range_string&
    );

    FSL_CHECK_EQ( fostlib::coerce< unsigned char >( u1 ), uc1 );
    FSL_CHECK_EQ( fostlib::coerce< unsigned char >( u2 ), uc2 );
    FSL_CHECK_EXCEPTION(
        fostlib::coerce< unsigned char >( u3 ),
        fostlib::exceptions::out_of_range_string&
    );
}
