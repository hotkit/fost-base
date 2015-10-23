/*
    Copyright 2001-2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_EXCEPTION_HPP
#define FOST_EXCEPTION_HPP
#pragma once


#include <fost/config.hpp>
#include <fost/string.hpp>
#include <fost/json-core.hpp>
#include <fost/accessors.hpp>
#include <exception>


namespace fostlib {


    namespace exceptions {


        /// Base class for all Fost exceptions
        class FOST_CORE_DECLSPEC FSL_ABSTRACT exception :
                public std::exception {
        public:
            /// Copy construct an exception
            exception( const exception & ) throw ();
            /// Allow this class to be safely used as a base class
            ~exception() throw ();

            /// Return the overall message for the exception
            virtual wliteral const message() const = 0;

            /// Print the exception on to the specified stream
            virtual ostream &printOn( ostream & ) const;
            /// Fetch the textual information about the exception
            const stringstream &info() const;
            /// Fetch the structured information about the exception
            const json &data() const;

            /// Used to add textual information about the exception
            stringstream &info();
            /// Used to store structured data in the exception
            json &data();

            /// Allow us to retrieve a text description of the exception as used by std::exception
            const char *what() const throw ();

            /// Return a JSON representation of this exception
            virtual json as_json() const;

        protected:
            stringstream m_info;
            json m_data;

            exception() throw ();
            explicit exception( const string & ) throw ();

        private:
            mutable boost::scoped_array< char > m_what_string;
        };


        /// This exception is thrown when CTRL-C or CTRL-BRK is pressed
        class FOST_CORE_DECLSPEC ctrl_break {
            public:
                ctrl_break();
        };


        class structure_information;
        class FOST_CORE_DECLSPEC structured_handler {
        public:
            structured_handler() throw ();
            ~structured_handler() throw ();

        public:
            std::unique_ptr< structure_information > m_info;
        };


       inline ostream &operator<<( ostream &o, const exception &e ) {
            return e.printOn( o );
        }


        /// An exception that is inflated from a JSON representation
        class FOST_CORE_DECLSPEC forwarded : public exception {
            /// Create from JSON
            forwarded(const json &);

            /// The exception name
            wliteral const message() const noexcept override;
        };


    }


    /// Call this whenever an exception is thrown away so that it break points can be set easily
    FOST_CORE_DECLSPEC void absorb_exception() throw ();


}


#ifdef WIN32
    #include <fost/exception/win.hpp>
#endif


#endif // FOST_EXCEPTION_HPP

