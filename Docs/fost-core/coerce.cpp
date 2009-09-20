/** \defgroup fost_core_coerce coerce
    \ingroup fost_core

    \brief A type safe type coercion library which is extensible for user defined types.

    The coercion library provides a mechanism for converting values from one type to another. It can be used as a safer alternative for many uses of static_cast as it will perform extra checking and throw more meaningful error messages (in the form of exceptions) if anything is wrong. Actual coercion is performed by fostlib::coerce and the coercions are implemented by fostlib::coercer.

    Coercions are only defined between types and both directions must be defined for general use, i.e. a coercion from string to integer must be defined as well as one from integer to string.

    The current implementation allows for coercions to use partial template specialisation which means whole <i>classes</i> of coercion can be implemented in terms of other coercions.
*/
/** \class fostlib::coercer coerce.hpp fost/core
    \ingroup fost_core_coerce
    \param T The type that is being coerced to.
    \param F The type that is being coerced from.
    \param Enable Used to enable coercion based on template meta-programming and SFINAE. See fostlib::coercer< int64_t, I > for an example.

    This struct together with the function fostlib::coerce is used to perform type coercions. To actually perform a coercion use the fostlib::coerce function, to define the possible coercions add a specialisation (or a partial specialisation) of this struct.

    <h2>Coercing to and from your own types</h2>

    The simplest form is to provide a specialisation of the fostlib::coercer struct. This defines a single member function that performs a coercion between the specified types.

    For example, to coerce from a wide character literal  or array pointer we simply need to use the fostlib::string constructor that deals with it:

    <pre class="language-cpp">
    namespace fostlib {
        template<>
        struct coercer< string, wliteral > {
            string coerce( wliteral str ) {
                return string( str );
            }
        };
    }
    </pre>

    It is also possible to perform a partial specialisation. For example, to convert between two nullable types Fost 4 defines the following:

    <pre class="language-cpp">
    namespace fostlib {
        template< typename T, typename F >
        struct coercer< nullable< T >, nullable< F > > {
            T coerce( const nullable< F > &f ) {
                if ( f.isnull() )
                    return null;
                else
                    return coercer< T, F >().coerce( f.value() );
            }
        };
    }
    </pre>

    This checks to see if we're dealing with a value which is null. If so it returns null. Otherwise it uses the coercer for the concrete type.

    <b>Note that if you are going to call fostlib::coerce from inside your own coercion specialisation you must fully qualify the function call because the internal coerce implementation is called "coerce" so it will be found rather than the coerce operator.</b> This is a poor choice of names and one we should change.
*/
/** \fn fostlib::coerce(const F &f)
    \ingroup fost_core_coerce
    \param T The type to be coerced to. This must be specified when the operator is used.
    \param F The type to be coerced from. The compiler will automatically detect this.
    \param f The instance of F that is to be coerced.

    The coercion operator is used like a standard C++ cast operator:

    <pre>fostlib::coerce< fostlib::string >( 10 ); // Turns the integer 10 into its string representation</pre>

    Coercion is intended to preserve as much information as possible about the originating type, so for example:

    <pre>fostlib::coerce< fostlib::string >( 3.141f ); // Gives "3.1410000324249268"</pre>

    This is because the requested value is not precisely storable as a double and the type coercion function performs no rounding. If you require human readable strings there are functions higher up in the library stack for this (which will be released in due course as they are ported for Fost 4).

    If a coercion is not defined then you will get an error. Both g++ and MSVC will give an error at or near fost/coerce.hpp line 22. There is no default coercion specified in order to maintain as much safety as possible when you need to convert between types. To learn how to define coercion for your own types see fostlib::coercer.
*/
