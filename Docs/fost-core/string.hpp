/** \defgroup fost_core_string string
    \ingroup fost_core

    \#include <fost/core>

    The string library provides a measure of source compatibility between string handling on Windows and Linux compilers, but more importantly, it abstracts out the differences between the two platforms native string types and both of their differences to the Unicode standard.

    It is designed to allow migration to the new C++0x Unicode handling with a minimum of fuss.
*/
/** \class fostlib::string string.hpp fost/core
    \ingroup fost_core_string

    A Unicode aware string class that uses the native string type internally, but only exposes a Unicode 4 compatible public interface.

    The string class itself has an interface very closely modelled on that of `std::basic_string<>` with a few changes:
    - All character oriented methods take and return characters of type utf32 rather than char or wchar_t. This means that character counts more closely model those that end users would expect to see. For example, the treble clef character has a count of one, rather than the two it gets on Windows (UTF-16) and the four it gets on Linux (UTF-8).
    - Iterators are only available in `const` form. As the underlying character buffer requires a non-constant number of the underlying character type to represent each UTF-32 code point it isn't possible to handle mutation with the expected iterator validity/lifetimes.

    There is also the addition of some new members:

    - fostlib::string::std_str() -- Returns a platform specific native string. On Linux this will be std::string and on Windows it will be std::wstring.

    <h3>String literals</h3>

    In order to handle portability until such time as enough compilers support the new Unicode string types it is recommended that all string literals are written with wide characters:

    <pre class="language-cpp">
    L"This is a wide character literal -- note the leading 'L'"
    "This is a narrow character literal -- it doesn't have any leading letters"
    </pre>

    It is not wise to embed anything other than ASCII characters in your source code. For any non-standard character the Unicode code point should be embedded. Unfortunatly for Windows portability you will need to UTF-16 encode the code point. For anything that you're likely to want to use day-to-day this is not an issue.

    <pre class="language-cpp">
    L"This odd character \x2014 is an m-dash"
    </pre>


    <h2>Examples</h2>

    <pre class="language-cpp">
    #include <iostream>
    #include <fost/core>

    int main() {
        fostlib::string str( L"Hello world" );
        std::cout << str << std::endl;
    }
    </pre>
*/
#include <fost/core>
