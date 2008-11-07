/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/dynlib.hpp>


#ifdef WIN32
    #include "dynlib-win.cpp"
#else
    #include "dynlib-linux.cpp"
#endif
