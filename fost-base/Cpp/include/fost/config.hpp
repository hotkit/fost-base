/*
    Copyright 2001-2014, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_CONFIG_HPP
#define FOST_CONFIG_HPP
#pragma once


// Some control #defines
#ifdef FOST_OS_WINDOWS
    #ifndef WINVER
        #define WINVER 0x0500 // Support only Windows 2000 and above
        #define _WIN32_WINNT WINVER
        #define _WIN32_WINDOWS WINVER
    #endif

    #define _ATL_APARTMENT_THREADED
    #define _ATL_NO_AUTOMATIC_NAMESPACE
    #define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
    #define _ATL_ALL_WARNINGS

    #define BOOST_ALL_DYN_LINK
    #define BOOST_THREAD_USE_DLL

    #ifdef _MSC_VER
        #include "config.msvc.hpp"
    #endif

    // Might need to be 1500 for MSVC 9.0
    #if ( _MSC_VER < 1600 )
        // In stdint.h on Linux
        typedef __int8 int8_t;
        typedef unsigned __int8 uint8_t;
        typedef __int16 int16_t;
        typedef unsigned __int16 uint16_t;
        typedef __int32 int32_t;
        typedef unsigned __int32 uint32_t;
        typedef __int64 int64_t;
        typedef unsigned __int64 uint64_t;
    #else
        #include <stdint.h>
    #endif

    #define FSL_ABSTRACT __declspec( novtable )
    #define FSL_EXPORT __declspec( dllexport )
#else
    #include <stdint.h>

    typedef long LONG;

    #define FSL_ABSTRACT
    #define FSL_EXPORT

    #undef Null
#endif


#include <limits>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <typeinfo>
#include <string>
#include <sstream>
#include <fstream>

#if INT_MAX != LONG_MAX && LONG_MAX != LLONG_MAX
    #define FOST_USE_LONG
#endif

#pragma GCC diagnostic ignored "-Wunused-local-typedefs"

#include <boost/utility.hpp>
#include <boost/smart_ptr.hpp>

#ifdef _MSC_VER
    #pragma warning ( push )
    #pragma warning ( disable : 4511 ) // Cannot generate copy constructors for tuples used by boost::lambda in boost 1.33.1
#endif
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#ifdef _MSC_VER
    #pragma warning ( pop )
#endif
#include <boost/function.hpp>
#include <boost/type_traits.hpp>

namespace boost {
    namespace detail {
        template<typename R, typename C>
        struct function_traits_helper<R (C::* *)(void) const >
        {
          BOOST_STATIC_CONSTANT(int, arity = 0);
          typedef R result_type;
          typedef C class_type;
        };
    }
}


#ifdef FOST_OS_WINDOWS
    #ifdef FOST_HAVE_MFC
        #include <afxwin.h>
        #include <afxdisp.h>
        #include <atlbase.h>
        #include <atlcom.h>
        #include <comdef.h>
    #else
        #define WINDOWS_LEAN_AND_MEAN 1
        #define _WINSOCKAPI_
        #include <windows.h>
        #undef _WINSOCKAPI_
    #endif
    // Microsoft are nuts -- the stuff they #define
    // It turns out we can't undefine it because loads of their headers rely on this
    // #undef interface
#endif


#endif // FOST_CONFIG_HPP

