/*
    Copyright 2010-2014, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <boost/type_traits.hpp>


FSL_TEST_SUITE( coercion_int );


namespace {
    class static_base_tests {
        BOOST_STATIC_ASSERT(( boost::is_integral< signed char >::value ));
        BOOST_STATIC_ASSERT(( boost::is_signed< signed char >::value ));
        BOOST_STATIC_ASSERT( sizeof(signed char) == 1 );
        BOOST_STATIC_ASSERT(( boost::is_integral< int64_t >::value ));
        BOOST_STATIC_ASSERT(( boost::is_signed< int64_t >::value ));
        BOOST_STATIC_ASSERT( sizeof(int64_t) == 8 );
        BOOST_STATIC_ASSERT(( boost::is_integral< boost::uintmax_t >::value ));
        BOOST_STATIC_ASSERT(( boost::is_unsigned< boost::uintmax_t >::value ));
        BOOST_STATIC_ASSERT( sizeof(boost::uintmax_t) == 8 );

        typedef boost::mpl::and_<
            boost::mpl::bool_< true >,
            boost::mpl::bool_< true >,
            boost::mpl::bool_< true >,
            boost::mpl::bool_< true >,
            boost::mpl::bool_< true >
        > boost_mpl_and;
        typedef boost::enable_if<
            boost::mpl::and_<
                boost::mpl::bool_< boost::is_integral< signed char >::value >,
                boost::mpl::bool_< boost::is_integral< int64_t >::value >,
                boost::mpl::bool_< ( sizeof(signed char) < sizeof(int64_t) ) >,
                boost::mpl::or_<
                    boost::mpl::and_<
                        boost::mpl::bool_< boost::is_signed< signed char >::value >,
                        boost::mpl::bool_< boost::is_signed< int64_t >::value >
                    >,
                    boost::mpl::and_<
                        boost::mpl::bool_< boost::is_unsigned< signed char >::value >,
                        boost::mpl::bool_< boost::is_unsigned< int64_t >::value >
                    >
                >
            >
        >::type test_coerce_int_T_smaller_F_larger;
    };
    class static_asserts {
        BOOST_STATIC_ASSERT((
            fostlib::detail::coerce_int_equal_size_same_sign<
                char, char
            >::value
        ));
        BOOST_STATIC_ASSERT((
            fostlib::detail::coerce_int_equal_size_same_sign<
                int, int
            >::value
        ));
        BOOST_STATIC_ASSERT((
            fostlib::detail::coerce_int_equal_size_same_sign<
                uint64_t, uint64_t
            >::value
        ));

        BOOST_STATIC_ASSERT((
            fostlib::detail::coerce_int_T_larger_F_smaller<
                uint64_t, unsigned char
            >::value
        ));
        BOOST_STATIC_ASSERT((
            fostlib::detail::coerce_int_T_larger_F_smaller<
                int64_t, signed char
            >::value
        ));

        BOOST_STATIC_ASSERT((
            fostlib::detail::coerce_int_T_smaller_F_larger_unsigned<
                unsigned char, uint64_t
            >::value
        ));
        BOOST_STATIC_ASSERT((
            fostlib::detail::coerce_int_T_smaller_F_larger_signed<
                signed char, int64_t
            >::value
        ));
        BOOST_STATIC_ASSERT((
            fostlib::detail::coerce_int_T_smaller_F_larger_signed<
                int, int64_t
            >::value
        ));

        BOOST_STATIC_ASSERT((
            fostlib::detail::coerce_int_T_smaller_signed_F_larger_unsigned<
                signed char, uint64_t
            >::value
        ));

        BOOST_STATIC_ASSERT((
            fostlib::detail::coerce_int_T_larger_signed_F_smaller_unsigned<
                int64_t, unsigned char
            >::value
        ));

        BOOST_STATIC_ASSERT((
            fostlib::detail::coerce_int_T_larger_unsigned_F_smaller_signed<
                uint64_t, signed char
            >::value
        ));

        BOOST_STATIC_ASSERT((
            fostlib::detail::coerce_int_T_smaller_unsigned_F_larger_signed<
                unsigned char, int64_t
            >::value
        ));
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


FSL_TEST_FUNCTION( from_same_size_with_different_sign ) {
    signed char c1 = 0, c2 = -10, c3 = 10;
    unsigned char uc1 = 0, uc2 = 10;
    int64_t i1 = 0, i2 = -10, i3 = 10, i4 = -1000, i5 = 1000;
    uint64_t u1 = 0, u2 = 10, u3 = 1000;

    FSL_CHECK_EQ( fostlib::coerce< unsigned char >( c1 ), c1 );
    FSL_CHECK_EXCEPTION(
        fostlib::coerce< unsigned char >( c2 ),
        fostlib::exceptions::out_of_range_string&
    );
    FSL_CHECK_EQ( fostlib::coerce< unsigned char >( c3 ), c3 );

    FSL_CHECK_EQ( fostlib::coerce< signed char >( uc1 ), c1 );
    FSL_CHECK_EQ( fostlib::coerce< signed char >( uc2 ), c3 );

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

    FSL_CHECK_EQ( fostlib::coerce< int64_t >( u1 ), i1 );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( u2 ), i3 );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( u3 ), i5 );
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
