/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/cli>
#include <fost/main.hpp>
#include <fost/inet>
#include <fost/internet/http.hpp>


using namespace fostlib;


FSL_MAIN(
    L"fget",
    L"Simple HTTP client\nCopyright (c) 2008, Felspar Co. Ltd."
)( fostlib::ostream &o, fostlib::arguments & ) {
    return 0;
}
