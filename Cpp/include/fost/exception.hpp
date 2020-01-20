/**
    Copyright 2001-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <fost/string.hpp>
#include <fost/json-core.hpp>
#include <fost/accessors.hpp>

#include <boost/system/error_code.hpp>

#include <exception>


namespace fostlib {


    namespace exceptions {


        /// Base class for all Fost exceptions
        class FOST_CORE_DECLSPEC exception : public std::exception {
          public:
            /// Copy construct an exception
            exception(const exception &) noexcept;
            /// Allow this class to be safely used as a base class
            ~exception() noexcept;

            /// Return the overall message for the exception
            virtual wliteral const message() const = 0;

            /// Print the exception on to the specified stream
            virtual ostream &printOn(ostream &) const;

            /// Fetch the structured information about the exception
            const json &data() const;

            /// Used to store structured data in the exception
            json &data();

            /// Allow us to retrieve a text description of the exception as used
            /// by std::exception
            const char *what() const noexcept;

            /// Return a JSON representation of this exception
            virtual json as_json() const;

          protected:
            json m_data;

            exception() noexcept;
            explicit exception(f5::u8view) noexcept;

          private:
            mutable std::unique_ptr<char> m_what_string;
        };


        /// This exception is thrown when CTRL-C or CTRL-BRK is pressed
        class FOST_CORE_DECLSPEC ctrl_break {
          public:
            ctrl_break();
        };


        class structure_information;
        class FOST_CORE_DECLSPEC structured_handler {
          public:
            structured_handler() noexcept;
            ~structured_handler() noexcept;

          public:
            std::unique_ptr<structure_information> m_info;
        };


        /// Output the exception to a stream
        inline ostream &operator<<(ostream &o, const exception &e) {
            return e.printOn(o);
        }


        /// An exception that is inflated from a JSON representation
        class FOST_CORE_DECLSPEC forwarded : public exception {
          public:
            /// Create from JSON
            forwarded(const json &);

            /// The exception name
            wliteral const message() const noexcept override;
        };


    }


    /// Call this whenever an exception is thrown away so that it break points
    /// can be set easily
    FOST_CORE_DECLSPEC void absorb_exception() noexcept;


}


#ifdef WIN32
#include <fost/exception/win.hpp>
#endif
