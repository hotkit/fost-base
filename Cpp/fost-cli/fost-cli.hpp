/**
    Copyright 2003-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#ifdef FOST_OS_WINDOWS
#define FOST_CLI_DECLSPEC __declspec(dllexport)
#else
#define FOST_CLI_DECLSPEC
#endif


#include <fost/core>
