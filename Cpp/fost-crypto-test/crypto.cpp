/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-crypto-test.hpp"
#include <fost/crypto>


FSL_TEST_SUITE( crypto );


FSL_TEST_FUNCTION( sha1_hmac ) {
    // http://docs.amazonwebservices.com/AmazonS3/2006-03-01/RESTAuthentication.html
    FSL_CHECK_EQ( fostlib::sha1_hmac(
        L"uV3F3YluFJax1cknvbcGwgjvx4QpvB+leU8dUj2o",
        L"GET\n\n\nTue, 27 Mar 2007 19:36:42 +0000\n/johnsmith/photos/puppy.jpg"
    ), L"xXjDGYUmKxnwqr5KXNPGldn5LbA=" );
    FSL_CHECK_EQ( fostlib::sha1_hmac(
        L"uV3F3YluFJax1cknvbcGwgjvx4QpvB+leU8dUj2o",
        L"PUT\n\nimage/jpeg\nTue, 27 Mar 2007 21:15:45 +0000\n/johnsmith/photos/puppy.jpg"
    ), L"hcicpDDvL9SsO6AkvxqmIWkmOuQ=" );
    FSL_CHECK_EQ( fostlib::sha1_hmac(
        L"uV3F3YluFJax1cknvbcGwgjvx4QpvB+leU8dUj2o",
        L"GET\n\n\nTue, 27 Mar 2007 19:42:41 +0000\n/johnsmith/"
    ), L"jsRt/rhG+Vtp88HrYL706QhE4w4=" );
    FSL_CHECK_EQ( fostlib::sha1_hmac(
        L"uV3F3YluFJax1cknvbcGwgjvx4QpvB+leU8dUj2o",
        L"PUT\n4gJE4saaMU4BqNR0kLY+lw==\napplication/x-download\nTue, 27 Mar 2007 21:06:08 +0000\nx-amz-acl:public-read\nx-amz-meta-checksumalgorithm:crc32\nx-amz-meta-filechecksum:0x02661779\nx-amz-meta-reviewedby:joe@johnsmith.net,jane@johnsmith.net\n/static.johnsmith.net/db-backup.dat.gz"
    ), L"C0FlOtU8Ylb9KDTpZqYkZPX91iI=" );
}

