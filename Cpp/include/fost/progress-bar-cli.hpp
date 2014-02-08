/*
    Copyright 2013, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_PROGRESS_BAR_CLI_HPP
#define FOST_PROGRESS_BAR_CLI_HPP
#pragma once


#include <fost/progress>


namespace fostlib {


    namespace cli {


        /// Return a string of the requested length representing the reading
        FOST_CLI_DECLSPEC string bar(const meter::reading &, std::size_t width);


    }


}


#endif // FOST_PROGRESS_BAR_CLI_HPP
