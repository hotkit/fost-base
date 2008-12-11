/*
    Copyright 2001-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_CONFIG_HPP
#define FOST_CONFIG_HPP
#pragma once


// Some control #defines
#ifdef WIN32
    #define WINVER 0x0500 // Support only Windows 2000 and above
    #define _WIN32_WINNT 0x0500
    #define _WIN32_WINDOWS 0x0500

    #define _ATL_APARTMENT_THREADED
    #define _ATL_NO_AUTOMATIC_NAMESPACE
    #define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
    #define _ATL_ALL_WARNINGS

    #define BOOST_ALL_DYN_LINK
    #define BOOST_THREAD_USE_DLL

    #ifdef _MSC_VER
        #include "config.msvc.hpp"
    #endif

    // In stdint.h on Linux
    typedef __int16 int16_t;
    typedef unsigned __int16 uint16_t;
    typedef __int32 int32_t;
    typedef unsigned __int32 uint32_t;
    typedef __int64 int64_t;
    typedef unsigned __int64 uint64_t;

    #define FSL_ABSTRACT __declspec( novtable )
#else
    #include <stdint.h>

    typedef long LONG;

    #define FSL_ABSTRACT

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

#if MAX_INT != MAX_LONG && MAX_LONG != MAX_LONG_LONG
    #define FOST_USE_LONG
#endif

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


#ifdef WIN32
    #include <afxwin.h>
    #include <afxdisp.h>
    #include <atlbase.h>
    #include <atlcom.h>
    #include <comdef.h>
    #include <asptlb.h>
#endif


#endif // FOST_CONFIG_HPP

