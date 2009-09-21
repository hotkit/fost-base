/** \defgroup fost_core_string string
    \ingroup fost_core

    \brief A Unicode string class modelled on the std::string interface, but using the native character size internally (char on Linux, wchar_t on Windows), but presenting a consistent UTF-32 based exterior. This allows Unicode strings to be used efficiently with portable source code between Windows and Linux.

    The string library provides a measure of source compatibility between string handling on Windows and Linux compilers, but more importantly, it abstracts out the differences between the two platforms native string types and both of their differences to the Unicode standard.

    It is designed to allow migration to the new C++0x Unicode handling with a minimum of fuss.

    \section tagged Tagged strings

    Tagged strings are a mechanism whereby strings can be given a type to restrict their interaction with other strings. They are used where formatted strings are required and the formatting precludes the free interchange with general sting types.
*/
/** \class fostlib::string string.hpp fost/core
    \ingroup fost_core_string

    A Unicode aware string class that uses the native string type internally, but only exposes a Unicode 4 compatible public interface.

    The string class itself has an interface very closely modelled on that of std::basic_string with a few changes:
    - All character oriented methods take and return characters of type utf32 rather than char or wchar_t. This means that character counts more closely model those that end users would expect to see. For example, the treble clef character has a count of one, rather than the two it gets on Windows (UTF-16) and the four it gets on Linux (UTF-8).
    - Iterators are only available in `const` form. As the underlying character buffer requires a non-constant number of the underlying character type to represent each UTF-32 code point it isn't possible to handle mutation with the expected iterator validity/lifetimes.

    There is also the addition of some new members:

    - fostlib::string::std_str()
    - fostlib::string::native_length()

    \section literals String literals

    In order to handle portability until such time as enough compilers support the new Unicode string types it is recommended that all string literals are written with wide characters:

    <pre class="language-cpp">
    L"This is a wide character literal -- note the leading 'L'"
    "This is a narrow character literal -- it doesn't have any leading letters"
    </pre>

    It is not wise to embed anything other than ASCII characters in your source code. For any non-standard character the Unicode code point should be embedded. Unfortunatly for Windows portability you will need to UTF-16 encode the code point. For anything that you're likely to want to use day-to-day this is not an issue.

    <pre class="language-cpp">
    L"This odd character \x2014 is an m-dash"
    </pre>

    Note that an earlier version of this type only allowed wide character literals. This is still reflected in some of the members of the class which only provide wide character literal versions. If you get a compile error with narrow literals try converting to the wide character literal first.

    \section examples Examples

    <pre class="language-cpp">
    #include <iostream>
    #include <fost/core>

    int main() {
        fostlib::string str( L"Hello world" );
        std::cout << str << std::endl;
    }
    </pre>
*/
/** \fn native_string fostlib::string::std_str () const

    \brief Returns a platform specific native string. On Linux/Mac this will be std::string and on Windows it will be std::wstring.
*/
/** \fn size_type fostlib::string::length () const

    \brief Returns the length of the string as counted by UTF-32 code points.
*/
/** \fn size_type fostlib::string::native_length () const

    \brief Returns the length of the string in the native format. On Linux/Mac this will be the number of UTF-8 bytes, on Windows this will be the number of UTF-16 code points. The trailing nil is never counted.
*/


/** \class fostlib::tagged_string "fost/detail/tagged-string.hpp" fost/string
    \ingroup fost_core_string
    \param Tag The tag type that defines how to encode the tagged string from the underlying string type and provides a check function to make sure that strings which are claimed to be pre-encoded meet the requisite rules.
    \param Impl The type of the underlying string implementation. This may itself be a tagged string.

    \brief A class which allows strings with incompatible formats to be given incompatible types.

    Tagged strings are used to impose rules on string types and to make strings with incompatible rules also incompatible with each other as C++ types. Tagged strings offer a sub-set of the full functionality of normal strings (std::basic_string or fostlib::string for example).

    For example, the length of a tagged string cannot be taken because it isn't clear which representation should provide the length. For example, an SQL string type would have a logical representation of "This string'll confuse", but the tagged string itself might store "This string''ll confuse". When this tagged string is converted to one representing an SQL statement then single quotes might be prepended and appended, i.e. "'This string''ll confuse'". Because it isn't clear which length is requested there is no length member on the tagged string.

    Note that the order of conversion might be important. Generally an application should manipulate strings as fostlib::string whenever possible and then convert to a tagged string at the last possible moment.

    Normally the \ref fost_core_coerce "coercion functions" are used to manage the transitions between tagged string types and normal Unicode strings. Where strings are known to be in specific formats already then they can be passed to the tagged string constructors directly.

    The tagged string type will work equally well with wide and narrow character types to provice the underlying storage. The constructors that take string literals default to assuming that the strings are pre-encoded, so when using string literals in source code care should be taken to ensure that the literal meets the rules for the tagged string.

    \section creating Creating your own tagged string types

    A tagged string is made up of two parts: the fostlib::tagged_string which provides the interface and underlying storage and a tag struct which provides functions for converting from the underlying string type and for checking that the tagged string conforms to its rules. For an example see fostlib::ascii_string_tag.
*/
