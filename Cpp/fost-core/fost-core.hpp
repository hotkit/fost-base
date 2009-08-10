/*
    Copyright  2003-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifdef WIN32
    #define FOST_CORE_DECLSPEC __declspec( dllexport )
#else
    #define FOST_CORE_DECLSPEC
#endif


#include <fost/exception.hpp>
#include <fost/coerce.hpp>
#include <fost/detail/coerce.hpp>
#include <fost/settings.hpp>
#include <fost/detail/utility-nullable.hpp>
