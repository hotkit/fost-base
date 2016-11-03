/*
    Copyright 2016, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma once


#include <fost/datetime>
#include <fost/sinks.hpp>


namespace fostlib {


    namespace detail {


        /// The file name for the specified log file
        class archive_pathname {
            /// Folder we save the log files into
            const boost::filesystem::wpath logfile_directory;
            /// The module part of the file name
            const module &module;
            /// The file size (beyond which) we will rotate the log files
            uintmax_t max_size_kb;

            /// Structure for storing information about the current logfile
            struct fileloc_type {
                fostlib::date date;
                boost::filesystem::wpath pathname;
            };
            /// The current log file name and its date
            fostlib::nullable<fileloc_type> fileloc;

            /// Generate an archive file name to use
            fileloc_type pathname(const fostlib::timestamp &) const;

        public:
            /// Construct an archive for the default data stream
            archive_pathname();

            /// Construct an archive for a given module name
            archive_pathname(const class module &);

            /// The path for the archive data in the Panoptes meta data database.
            const fostlib::jcursor &meta_db_path() const;

            /// Return the file name for the given timestamp
            boost::filesystem::wpath operator () (const fostlib::timestamp &);

            /// Return true if the file needs to be rotated
            bool rotate(uintmax_t);
        };


    }


}

