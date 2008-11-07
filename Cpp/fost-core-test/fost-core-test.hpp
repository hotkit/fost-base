/*
    Copyright 2003-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifdef WIN32
    #define FOST_CORE_DECLSPEC __declspec( dllimport )
    #define FOST_TEST_DECLSPEC __declspec( dllimport )
#else
    #define FOST_CORE_DECLSPEC
    #define FOST_TEST_DECLSPEC
#endif


#include <fost/exception.hpp>
#include <fost/coerce.hpp>
#include <fost/string/coerce.hpp>
#include <fost/settings.hpp>
#include <fost/string/utility-nullable.hpp>
#include <fost/test.hpp>
