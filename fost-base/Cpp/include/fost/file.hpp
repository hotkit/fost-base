/*
    Copyright 2001-2013, Felspar Co Ltd. http://support.felspar.com/
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
        FOST_CORE_DECLSPEC string load_file(
            const boost::filesystem::wpath &filename);
        /// Load a text file which is UTF-8 encoded, giving default content to use if there is an error
        FOST_CORE_DECLSPEC string load_file(
            const boost::filesystem::wpath &filename,
            const string &default_content);

        /// Save a string into a file UTF-8 encoded
        FOST_CORE_DECLSPEC void save_file(
            const boost::filesystem::wpath &filename,
            const string &content);


    }


#if ( BOOST_VERSION_MAJOR < 44 )
    /// Use the wpath specialisation of the version 2 directory iterator
    typedef boost::filesystem::basic_directory_iterator<
        boost::filesystem::wpath> directory_iterator;
#else
    /// We can just use the normal version 3 directory iterator
    typedef boost::filesystem::directory_iterator directory_iterator;
#endif


    /// Coerce a string to a file path
    template<>
    struct coercer< boost::filesystem::wpath, string > {
        boost::filesystem::wpath coerce( const string &s ) {
            return fostlib::coerce< std::wstring >( s );
        }
    };
    /// Coerce a file path to a string
    template<>
    struct coercer< string, boost::filesystem::wpath > {
        string coerce( const boost::filesystem::wpath &p ) {
#if (BOOST_VERSION_MAJOR < 44)
            return fostlib::coerce< string >( p.string() );
#else
            return fostlib::coerce< string >( p.wstring() );
#endif
        }
    };
    /// Coerce a file path to JSON
    template<>
    struct coercer< json, boost::filesystem::wpath > {
        json coerce( const boost::filesystem::wpath &p ) {
            return json( fostlib::coerce< string >( p ) );
        }
    };
    /// Coerce JSON to a file path
    template<>
    struct coercer< boost::filesystem::wpath, fostlib::json > {
        boost::filesystem::wpath coerce( const json &j ) {
            return boost::filesystem::wpath(
                fostlib::coerce< std::wstring >(
                    fostlib::coerce< string >( j )));
        }
    };


    /// Return a path unique pathname
    FOST_CORE_DECLSPEC boost::filesystem::wpath unique_filename();


}


namespace std {


    /// Allow a wpath to be printed to an ostream
    inline fostlib::ostream &operator << (
        fostlib::ostream &o, const boost::filesystem::wpath &p
    ) {
        return o << fostlib::coerce< fostlib::string >( p );
    }


}


#endif // FOST_FILE_HPP

