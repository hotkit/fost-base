/**
    Copyright 2001-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#ifndef FOST_FILE_HPP
#define FOST_FILE_HPP
#pragma once


#include <fost/string-fwd.hpp>
#include <fost/filesystem.hpp>


namespace fostlib {


    namespace utf {


        /// Load a text file which is UTF-8 encoded
        FOST_CORE_DECLSPEC string load_file(const fostlib::fs::path &filename);
        /// Load a text file which is UTF-8 encoded, giving default content to
        /// use if there is an error
        FOST_CORE_DECLSPEC string load_file(
                const fostlib::fs::path &filename,
                const string &default_content);

        /// Save a string into a file UTF-8 encoded
        FOST_CORE_DECLSPEC void save_file(
                const fostlib::fs::path &filename, const string &content);


    }


    typedef fostlib::fs::directory_iterator directory_iterator;


    /// Coerce a string to a file path
    template<>
    struct coercer<fostlib::fs::path, string> {
        fostlib::fs::path coerce(const string &s) {
            return static_cast<std::string>(s);
        }
    };
    /// Coerce a file path to a string
    template<>
    struct coercer<string, fostlib::fs::path> {
        string coerce(const fostlib::fs::path &p) {
            return fostlib::coerce<string>(p.wstring());
        }
    };
    /// Coerce a file path to JSON
    template<>
    struct coercer<json, fostlib::fs::path> {
        json coerce(const fostlib::fs::path &p) {
            return json(fostlib::coerce<string>(p));
        }
    };
    /// Coerce JSON to a file path
    template<>
    struct coercer<fostlib::fs::path, fostlib::json> {
        fostlib::fs::path coerce(const json &j) {
            return fostlib::coerce<fostlib::fs::path>(
                    fostlib::coerce<string>(j));
        }
    };


    /// Return a path unique pathname
    FOST_CORE_DECLSPEC fostlib::fs::path unique_filename();


    /// Join two paths. If path is rooted it is returned, otherwise it is joined
    /// to root
    FOST_CORE_DECLSPEC fostlib::fs::path join_paths(
            const fostlib::fs::path &root, const fostlib::fs::path &path);


}


namespace std {


    /// Allow a path to be printed to an ostream
    inline fostlib::ostream &
            operator<<(fostlib::ostream &o, const fostlib::fs::path &p) {
        return o << fostlib::coerce<fostlib::string>(p);
    }


}


#endif // FOST_FILE_HPP
