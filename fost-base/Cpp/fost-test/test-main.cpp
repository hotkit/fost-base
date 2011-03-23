/*
    Copyright 2005-2011, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/main>
#include <fost/test>


namespace {
    fostlib::setting< bool > c_verbose( L"fost-test/test-main.cpp", L"Tests", L"Display test names", true );
}


FSL_MAIN(
    L"test-runner",
    L"test-runner test executor\nCopyright (c) 1995-2011, Felspar Co. Ltd."
)( fostlib::ostream &out, fostlib::arguments &args ) {
    if ( fostlib::test::suite::execute( out ) )
        return 1;
    else
        return 0;
}
