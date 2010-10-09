/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_TIMEZONE_HPP
#define FOST_TIMEZONE_HPP
#pragma once


#include <fost/core>


namespace fostlib {


    class FOST_CORE_DECLSPEC zoneinfo {
        public:
            zoneinfo();

            accessors< string > name;
    };


}


namespace std {


    inline fostlib::ostream &operator << ( fostlib::ostream &o, fostlib::zoneinfo t ) {
        return o << t.name();
    }


}


#endif // FOST_TIMEZONE_HPP

