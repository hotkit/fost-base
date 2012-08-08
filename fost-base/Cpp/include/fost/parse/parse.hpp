/*
    Copyright 2007-2012, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_PARSE_PARSE_HPP
#define FOST_PARSE_PARSE_HPP


// Used for configuring Boost 1.38.0
#define BOOST_SPIRIT_USE_OLD_NAMESPACE


#ifdef WIN32
    // comma operator within array index expression
    #pragma warning ( disable : 4709 )
    //conversion from 'int' to 'FSLib::utf16', possible loss of data
    #pragma warning ( disable : 4244 )
#endif

#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wmismatched-tags"
#endif

#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/phoenix1.hpp>

#ifdef __clang__
    #pragma clang diagnostic pop
#endif

#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include <fost/detail/tagged-string.hpp>


namespace fostlib {


    struct utf16_string_builder_closure : boost::spirit::closure< utf16_string_builder_closure,
        fostlib::string, std::vector< wchar_t >, wchar_t
    > {
        member1 text;
        member2 buffer;
        member3 character;
    };
    struct utf8_string_builder_closure : boost::spirit::closure< utf8_string_builder_closure,
        fostlib::utf8_string, std::vector< utf8 >, utf8
    > {
        member1 text;
        member2 buffer;
        member3 character;
    };
    struct ascii_string_builder_closure : boost::spirit::closure< ascii_string_builder_closure,
        fostlib::ascii_string, std::vector< char >, char
    > {
        member1 text;
        member2 buffer;
        member3 character;
    };
    struct ascii_printable_string_builder_closure : boost::spirit::closure< ascii_printable_string_builder_closure,
        fostlib::ascii_printable_string, std::vector< char >, char
    > {
        member1 text;
        member2 buffer;
        member3 character;
    };


    namespace detail {


        /// Returns a mutex used to serialise access to the Boost Spirit parsers
        FOST_CORE_DECLSPEC
        boost::recursive_mutex &g_parser_mutex();


        // Implementation taken from
        // http://spirit.sourceforge.net/distrib/spirit_1_8_5/libs/spirit/example/fundamental/stuff_vector.cpp
        struct push_back_impl {
            template <typename Container, typename Item>
            struct result {
                typedef void type;
            };
            template <typename Container, typename Item>
            void operator()(Container& c, Item const& item) const {
                c.push_back(item);
            }
            void operator()(json& c, json const& item) const {
                jcursor().push_back( c, item );
            }
        };

        struct insert_impl {
            template <typename Container, typename Key, typename Value>
            struct result {
                typedef void type;
            };
            template <typename Container, typename Key, typename Value>
            void operator()(Container& c, Key const& key, Value const& value) const {
                c.insert( key, value );
            }
            void operator()( json &c, string const& key, json const& value) const {
                (jcursor( key ))( c ) = value;
            }
        };

        template< typename To >
        struct coerce_impl {
            template< typename From >
            struct result {
                typedef To type;
            };
            template< typename From >
            To operator () ( const From &f ) {
                return fostlib::coerce< To >( f );
            }
        };

    }

    namespace parsers {


        const phoenix::function< fostlib::detail::push_back_impl > push_back =
            fostlib::detail::push_back_impl();
        const phoenix::function< fostlib::detail::insert_impl > insert =
            fostlib::detail::insert_impl();

        template< typename To >
        phoenix::function< fostlib::detail::coerce_impl< To > > coerce() {
            return fostlib::detail::coerce_impl< To >();
        }


    }


    /// RAII wrapper for the parser lock to serialise parses
    class parser_lock {
        boost::recursive_mutex::scoped_lock lock;
    public:
        parser_lock()
        : lock(detail::g_parser_mutex()) {
        }
    };


    /// Wrapper for boost::spirit::parse which forces serialisation of the parsing
    template<typename C, typename D> inline
    boost::spirit::parse_info<C> parse(
            C s, const boost::spirit::parser<D> &p) {
        parser_lock();
        return boost::spirit::parse(s, p);
    }


    /// Wrapper for boost::spirit::parse which forces serialisation of the parsing taking a previously acquired lock
    template<typename C, typename D> inline
    boost::spirit::parse_info<C> parse(
            parser_lock &lock,
            C s, const boost::spirit::parser<D> &p) {
        parser_lock();
        return boost::spirit::parse(s, p);
    }


}


#endif // FOST_PARSE_PARSE_HPP
