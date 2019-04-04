/**
    Copyright 2019, Felspar Co Ltd. <https://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#ifdef FSL_FORCE_STD_FILESYSTEM


#include <filesystem>


namespace fostlib {


    namespace fs = std::filesystem;
    using error_code = std::error_code;
    using ifstream = std::ifstream;
    using ofstream = std::ofstream;

    inline auto last_write_time_as_time_t(const std::filesystem::path &p) {
        auto const ftime = std::filesystem::last_write_time(p);
        return decltype(ftime)::clock::to_time_t(ftime);
    }


}


#else


#include <boost/filesystem.hpp>


namespace fostlib {


    namespace fs = boost::filesystem;
    using error_code = boost::system::error_code;
    using ifstream = boost::filesystem::ifstream;
    using ofstream = boost::filesystem::ofstream;

    inline auto last_write_time_as_time_t(const boost::filesystem::path &p) {
        return fs::last_write_time(p);
    }


}


#endif
