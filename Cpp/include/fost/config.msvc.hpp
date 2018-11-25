/*
    Copyright  2001-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma warning(disable : 4018) // Signed/Unsigned mismatch
#pragma warning(disable : 4096) // __cdecl must be used with '...'
#pragma warning(disable : 4146) // Unary minus on unsigned type
#pragma warning(disable : 4180) // Qualifier applied to function type has no
                                // meaning; ignored
#pragma warning(disable : 4231) // Nonstandard extension used : 'extern' before
                                // template explicit instantiation
#pragma warning(disable : 4251) // class '...' needs to have dll-interface to be
                                // used by clients of class '...'
#pragma warning(disable : 4275) // non - DLL-interface classkey '...' used as
                                // base for DLL-interface classkey '...'
#pragma warning(disable : 4290) // C++ Exception Specification ignored
#pragma warning( \
        disable : 4503) // Decorated name length exceeded, name was truncated
#pragma warning(disable : 4660) // Template-class specialization '...' is
                                // already instantiated
#pragma warning(disable : 4786) // Identifier truncated to 255 characters
#pragma warning( \
        disable : 4800) // Short forcing value to Bool - performance warning

#define NOMINMAX // Stops the max/min name clashes between MFC and
                 // Progress::max/min members

#define _SCL_SECURE_NO_DEPRECATE // Used to set MSVC libraries into a Boost
                                 // compatible mode
