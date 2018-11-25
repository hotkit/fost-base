/*
    Copyright 2003-2013,Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifdef FOST_OS_WINDOWS
#define FOST_CORE_DECLSPEC __declspec(dllexport)
#else
#define FOST_CORE_DECLSPEC
#endif


#include <fost/exception.hpp>
#include <fost/coerce.hpp>
#include <fost/detail/coerce.hpp>
#include <fost/settings.hpp>
#include <fost/utility-nullable.hpp>
