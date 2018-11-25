/*
    Copyright 2001-2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_FILE_HPP
#define FOST_FILE_HPP
#pragma once


#include <fost/string-fwd.hpp>
#include <boost/filesystem.hpp>


namespace fostlib {


    namespace utf {


        /// Load a text file which is UTF-8 encoded
        FOST_CORE_DECLSPEC string
                load_file(const boost::filesystem::path &filename);
        /// Load a text file which is UTF-8 encoded, giving default content to
        /// use if there is an error
        FOST_CORE_DECLSPEC string load_file(
                const boost::filesystem::path &filename,
                const string &default_content);

        /// Save a string into a file UTF-8 encoded
        FOST_CORE_DECLSPEC void save_file(
                const boost::filesystem::path &filename, const string &content);


    }


    typedef boost::filesystem::directory_iterator directory_iterator;


    /// Coerce a string to a file path
    template<>
    struct coercer<boost::filesystem::path, string> {
        boost::filesystem::path coerce(const string &s) {
#ifdef ANDROID
            return fostlib::coerce<utf8_string>(s).underlying();
#else
            return fostlib::coerce<std::wstring>(s);
#endif
        }
    };
    /// Coerce a file path to a string
    template<>
    struct coercer<string, boost::filesystem::path> {
        string coerce(const boost::filesystem::path &p) {
            return fostlib::coerce<string>(p.wstring());
        }
    };
    /// Coerce a file path to JSON
    template<>
    struct coercer<json, boost::filesystem::path> {
        json coerce(const boost::filesystem::path &p) {
            return json(fostlib::coerce<string>(p));
        }
    };
    /// Coerce JSON to a file path
    template<>
    struct coercer<boost::filesystem::path, fostlib::json> {
        boost::filesystem::path coerce(const json &j) {
            return fostlib::coerce<boost::filesystem::path>(
                    fostlib::coerce<string>(j));
        }
    };


    /// Return a path unique pathname
    FOST_CORE_DECLSPEC boost::filesystem::path unique_filename();


    /// Join two paths. If path is rooted it is returned, otherwise it is joined
    /// to root
    FOST_CORE_DECLSPEC boost::filesystem::path join_paths(
            const boost::filesystem::path &root,
            const boost::filesystem::path &path);


}


namespace std {


    /// Allow a path to be printed to an ostream
    inline fostlib::ostream &
            operator<<(fostlib::ostream &o, const boost::filesystem::path &p) {
        return o << fostlib::coerce<fostlib::string>(p);
    }


}


#endif // FOST_FILE_HPP
