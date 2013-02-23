/*
    Copyright 2007-2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_TEST_HPP
#define FOST_TEST_HPP
#pragma once


#include <fost/pointers>
#include <fost/exception.hpp>
#include <fost/string.hpp>
#include <fost/thread.hpp>

#include <fost/insert>


namespace fostlib {


    namespace exceptions {


        class FOST_TEST_DECLSPEC test_failure : public exception {
        public:
            test_failure( const string &condition, nliteral file, int64_t line );
            const wchar_t *const message() const;
        };


    }


    namespace test {


        class test;

        class FOST_TEST_DECLSPEC suite {
        public:
            suite( const fostlib::string &name );

            static bool execute();
            static bool execute( ostream & );

            typedef threadsafe_store< fostlib::reference_ptr< const test > > test_library_type;
            typedef test_library_type::keys_t test_keys_type;
            typedef test_library_type::found_t tests_type;

            test_keys_type test_keys() const {
                return m_tests.keys();
            }
            tests_type tests( const fostlib::string &name ) const {
                return m_tests.find( name );
            }
        private:
            void add( const fostlib::string &name, const test * ) const;

            friend class test;

            fostlib::string m_name;
            mutable test_library_type m_tests;
        };


        /// Represents a single test which is part of the suite
        class FOST_TEST_DECLSPEC test {
        public:
            /// Construct the test and add to the suite
            test(const suite &suite, const fostlib::string &name);
            /// Allow sub-classing to work properly
            virtual ~test() {}

            /// Execute the test
            void execute() const;

        private:
            /// The actual test implementation
            virtual void execute_inner() const = 0;
        };


        template <class T>
        T relative_error(T a, T b)
        {
           //
           // If math.h has no long double support we can't rely
           // on the math functions generating exponents outside
           // the range of a double:
           //
           T min_val = static_cast<T>((std::numeric_limits<T>::min)());
           T max_val = static_cast<T>((std::numeric_limits<T>::max)());

           if((a != 0) && (b != 0))
           {
              // TODO: use isfinite:
              if(fabs(b) >= max_val)
              {
                 if(fabs(a) >= max_val)
                    return 0;  // one infinity is as good as another!
              }
              // If the result is denormalised, treat all denorms as equivalent:
              if((a < min_val) && (a > 0))
                 a = min_val;
              else if((a > -min_val) && (a < 0))
                 a = -min_val;
              if((b < min_val) && (b > 0))
                 b = min_val;
              else if((b > -min_val) && (b < 0))
                 b = -min_val;
              return (std::max)(fabs((a-b)/a), fabs((a-b)/b));
           }

           // Handle special case where one or both are zero:
           if(min_val == 0)
              return fabs(a-b);
           if(fabs(a) < min_val)
              a = min_val;
           if(fabs(b) < min_val)
              b = min_val;
           return (std::max)(fabs((a-b)/a), fabs((a-b)/b));
        }


    }


}


#ifdef FOST_OS_WINDOWS
    #define FSL_TEST_EXPORT __declspec( dllexport )
#else
    #define FSL_TEST_EXPORT
#endif


#define FSL_TEST_SUITE( name ) \
    const struct FSL_TEST_EXPORT suite_##name : public fostlib::test::suite { \
        suite_##name() : suite( fostlib::string( #name ) ) {} \
    } g_suite

#define FSL_TEST_FUNCTION( name ) namespace {\
    const struct FSL_TEST_EXPORT test_##name : public fostlib::test::test { \
        test_##name() : test( g_suite, fostlib::string( #name ) ) {} \
            private: \
                void execute_inner() const; \
        } g_test##name; \
    } \
    void ::test_##name::execute_inner() const

#define FSL_CHECK( condition ) {\
    bool result( false );\
    try {\
        result = (condition);\
    } catch ( fostlib::exceptions::exception &e ) {\
        e.info() << L"Location: " << fostlib::string( __FILE__ ) << L": " << __LINE__ << std::endl;\
        throw;\
    } catch ( std::exception &e ) { \
        throw fostlib::exceptions::test_failure( fostlib::string( e.what() ), __FILE__, __LINE__ ); \
    } catch ( ... ) {\
        throw fostlib::exceptions::test_failure( fostlib::string( "Unknown exception type caught" ), __FILE__, __LINE__ );\
    }\
    if ( !result ) throw fostlib::exceptions::test_failure( fostlib::string( "Condition: " #condition ), __FILE__, __LINE__ );\
}

#define FSL_CHECK_NULL( condition ) { \
    bool result( false ); \
    try { \
        result = ( (condition) == fostlib::null ); \
    } catch ( fostlib::exceptions::exception &e ) { \
        e.info() << L"Location: " << fostlib::string( __FILE__ ) << L": " << __LINE__ << std::endl; \
        throw; \
    } catch ( std::exception &e ) { \
        throw fostlib::exceptions::test_failure( fostlib::string( e.what() ), __FILE__, __LINE__ ); \
    } catch ( ... ) {\
        throw fostlib::exceptions::test_failure( fostlib::string( "Unknown exception type caught" ), __FILE__, __LINE__ );\
    }\
    if ( !result ) { \
        fostlib::exceptions::test_failure failure( fostlib::string( "Not null: " #condition ), __FILE__, __LINE__ );\
        failure.info() << L"Result: " << ( condition ).value() << std::endl; \
        throw failure; \
    } \
}

namespace fostlib {
    namespace test {
        namespace detail {


            template< typename V >
            inline json convert_test_result(const V &v) {
                fostlib::stringstream ss;
                ss << v;
                return json(ss.str());
            }
            inline const json &convert_test_result(const json &j) {
                return j;
            }


            template< typename T, typename L, typename R >
            inline void perform_test(
                T test_operator, const char *test_name,
                const L &left, const R&right,
                const char *left_text, const char *right_text,
                const char *file, int64_t line
            ) {
                bool result( false );
                try {
                    result = test_operator(left, right);
                } catch ( exceptions::exception &e ) {
                    e.info() << L"Location: " << string( file ) << L": " << line << '\n';
                    throw;
                } catch ( std::exception &e ) {
                    throw exceptions::test_failure( string( e.what() ), file, line );
                } catch ( ... ) {
                    throw exceptions::test_failure(
                        string("Unknown exception type caught"), file, line);
                }
                if ( !result ) {
                    exceptions::test_failure failure(
                        string( test_name) + ": " + left_text + " and " + right_text, file, line);
                    insert(failure.data(), "test", "name", test_name);
                    insert(failure.data(), test_name, "left", "expression", left_text);
                    insert(failure.data(), test_name, "left", "result", convert_test_result(left));
                    insert(failure.data(), test_name, "right", "expression", right_text);
                    insert(failure.data(), test_name, "right", "result", convert_test_result(right));
                    throw failure;
                }
            }

            template< typename L, typename R >
            inline bool check_eq(const L &left, const R&right) {
                return (left == right);
            }
            template< typename L, typename R >
            inline bool check_neq(const L &left, const R&right) {
                return (left != right);
            }

            template< typename L, typename R >
            inline void eq(
                const L &left, const R&right,
                const char *left_text, const char *right_text,
                const char *file, int64_t line
            ) {
                perform_test(&(check_eq<L, R>), "equals", left, right,
                    left_text, right_text, file, line);
            }

            template< typename L, typename R >
            inline void neq(
                const L &left, const R&right,
                const char *left_text, const char *right_text,
                const char *file, int64_t line
            ) {
                perform_test(&(check_neq<L, R>), "not-equals", left, right,
                    left_text, right_text, file, line);
            }


        }
    }
}
#define FSL_CHECK_EQ( left, right ) \
    fostlib::test::detail::eq(left, right, #left, #right, __FILE__, __LINE__ )

#define FSL_CHECK_NEQ( left, right ) \
    fostlib::test::detail::neq(left, right, #left, #right, __FILE__, __LINE__)

#define FSL_CHECK_EXCEPTION( code, exct ) {\
    bool threw( false );\
    try {\
        code;\
    } catch( exct ) {\
        threw = true;\
    } catch ( fostlib::exceptions::exception &e ) {\
        throw fostlib::exceptions::test_failure( L"Caught " + fostlib::string( typeid( e ).name() ) + L" and should have been " + fostlib::string( typeid( exct ).name() ) + L" in: " + fostlib::string( #code ) + L"\n" + fostlib::coerce<fostlib::string>(e), __FILE__, __LINE__ );\
    } catch ( std::exception &e ) { \
        throw fostlib::exceptions::test_failure( fostlib::string( typeid( e ).name() ) + L" : " + fostlib::string( e.what() ) + L" in " + fostlib::string( #code ), __FILE__, __LINE__ ); \
    } catch ( ... ) {\
        throw fostlib::exceptions::test_failure( fostlib::string( "Unknown exception type thrown then caught: " #code ), __FILE__, __LINE__ );\
    }\
    if ( !threw ) throw fostlib::exceptions::test_failure( fostlib::string( "No exception: " #code ), __FILE__, __LINE__ );\
}

#define FSL_CHECK_NOTHROW( code ) {\
    try { \
        code; \
    } catch ( fostlib::exceptions::exception &e ) {\
        e.info() << L"Expression: " << fostlib::string( #code ) << L"\nLocation: " << fostlib::string( __FILE__ ) << L": " << __LINE__ << std::endl; \
        throw;\
    } catch ( std::exception &e ) { \
        throw fostlib::exceptions::test_failure( fostlib::string( typeid( e ).name() ) + L" : " + fostlib::string( e.what() ) + L" in " + fostlib::string( #code ), __FILE__, __LINE__ ); \
    } catch ( ... ) {\
        throw fostlib::exceptions::test_failure( fostlib::string( "Unknown exception type thrown: " #code ), __FILE__, __LINE__ );\
    } \
}

namespace fostlib {


    namespace test {


        /*
            Abstracted tests
        */


        template< typename T >
        void default_copy_constructable() {
            FSL_CHECK( T() == T() );
            FSL_CHECK( !( T() != T() ) );

            T def;
            def = T();
            FSL_CHECK( def == T() );
        }

        template< typename T >
        void default_isnull() {
            T def;
            FSL_CHECK( def.isnull() );
            FSL_CHECK( def == fostlib::null );
            FSL_CHECK( !( def != fostlib::null ) );
            FSL_CHECK( fostlib::null == def );
            FSL_CHECK( !( fostlib::null != def ) );
            FSL_CHECK_EXCEPTION( def.value(), exceptions::null& );
       }


    }


}


#endif // FOST_TEST_HPP
