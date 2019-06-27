/**
    Copyright 2008-2019, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#ifndef FOST_DYNLIB_HPP
#define FOST_DYNLIB_HPP


#include <memory>


namespace fostlib {


    class FOST_CORE_DECLSPEC dynlib final {
        struct impl;
        std::unique_ptr<impl> m_lib;

      public:
        dynlib(const string &path);
        ~dynlib();

        /// Not copyable
        dynlib(const dynlib &) = delete;
        dynlib &operator=(const dynlib &) = delete;
    };


}


#endif // FOST_DYNLIB_HPP
